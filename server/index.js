const http = require("node:http");
const fs = require("node:fs");
const path = require("node:path");
const { spawnSync } = require("node:child_process");

const ROOT = path.resolve(__dirname, "..");
const PORT = 4000;

function csvRows(file) {
  const text = fs.readFileSync(path.join(ROOT, "data", file), "utf8").trim();
  const lines = text.split(/\r?\n/);
  const split = (line) => {
    const out=[]; let value="", quoted=false;
    for(let i=0;i<line.length;i++){
      const c=line[i];
      if(c==='"' && line[i+1]==='"' && quoted){value+='"';i++;}
      else if(c==='"') quoted=!quoted;
      else if(c==="," && !quoted){out.push(value);value="";}
      else value+=c;
    }
    out.push(value); return out;
  };
  const headers=split(lines.shift());
  return lines.filter(Boolean).map(line=>{
    const vals=split(line); return Object.fromEntries(headers.map((h,i)=>[h,vals[i]??""]));
  });
}
function readData() {
  const issues=csvRows("issues.csv").map(r=>({
    id:Number(r.id), title:r.title, desc:r.description,
    skills:(r.required_skills||"").split(";").filter(Boolean),
    priority:Number(r.priority), hours:Number(r.estimated_hours),
    deadline:r.deadline, status:r.status
  }));
  const developers=csvRows("developers.csv").map(r=>({
    id:Number(r.id),name:r.name,hours:Number(r.available_hours),
    workload:Number(r.current_workload),experience:r.experience_level,skills:[]
  }));
  for(const s of csvRows("developer_skills.csv")){
    const d=developers.find(x=>x.id===Number(s.developer_id));
    if(d)d.skills.push({name:s.skill,level:s.proficiency});
  }
  const dependencies=csvRows("dependencies.csv").map(r=>({
    issueId:Number(r.issue_id),dependsOn:Number(r.depends_on_issue_id)
  }));
  return {issues,developers,dependencies};
}
function json(res,status,data){
  res.writeHead(status,{"Content-Type":"application/json","Access-Control-Allow-Origin":"*"});
  res.end(JSON.stringify(data));
}
function parseAssignments(output) {
  const start=output.indexOf("========== DEVELOPER ASSIGNMENT REPORT ==========");
  const end=output.indexOf("========== ASSIGNMENT SUMMARY ==========");
  if(start<0||end<0)return [];
  const section=output.slice(start,end);
  const blocks=section.split(/(?=Issue ID:)/).filter(x=>x.includes("Issue ID:"));
  return blocks.map(block=>{
    const get=(re)=>{const m=block.match(re);return m?m[1].trim():"";};
    const issueId=Number(get(/Issue ID:\s*(\d+)/));
    const developerIdText=get(/Developer ID:\s*(\d+)/);
    return {
      issueId,
      developerName:get(/Developer:\s*([^\n]+)/)||"",
      developerId:developerIdText?Number(developerIdText):-1,
      hours:Number(get(/Assigned Hours:\s*(\d+)/)||0),
      status:get(/Status:\s*([^\n]+)/)||"Unassigned",
      reason:get(/Reason:\s*([^\n]+)/)||""
    };
  }).filter(a=>a.issueId);
}
function runEngine() {
  const compile=spawnSync("g++",[
    "-std=c++17","-include","algorithm","-Iinclude",
    "src/main.cpp","src/CSVreader.cpp","src/Validator.cpp","src/Assignment.cpp",
    "-o","app"
  ],{cwd:ROOT,encoding:"utf8"});
  if(compile.status!==0) throw new Error("C++ compilation failed:\n"+(compile.stderr||compile.stdout));
  const run=spawnSync(path.join(ROOT,"app"),[],{cwd:ROOT,encoding:"utf8",maxBuffer:10*1024*1024});
  if(run.error) throw run.error;
  if(run.status!==0) throw new Error("C++ engine exited with an error:\n"+run.stderr);
  const output=run.stdout||"";
  return {output,assignments:parseAssignments(output)};
}
const server=http.createServer((req,res)=>{
  if(req.method==="OPTIONS"){res.writeHead(204,{"Access-Control-Allow-Origin":"*","Access-Control-Allow-Methods":"GET,POST,OPTIONS","Access-Control-Allow-Headers":"Content-Type"});return res.end();}
  if(req.url==="/api/health")return json(res,200,{ok:true});
  if(req.url==="/api/data"&&req.method==="GET"){
    try{return json(res,200,readData());}
    catch(e){return json(res,500,{error:e.message});}
  }
  if(req.url==="/api/run"&&req.method==="POST"){
    try{return json(res,200,runEngine());}
    catch(e){return json(res,500,{error:e.message});}
  }
  json(res,404,{error:"Not found"});
});
server.listen(PORT,()=>console.log(`Issue Triage API running at http://localhost:${PORT}`));
