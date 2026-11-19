import { useEffect, useMemo, useState } from "react";
import {
  Activity, AlertCircle, ArrowDownRight, ArrowUpRight, Bell, BookOpen,
  Check, CheckCircle2, ChevronDown, CircleDot, Code2, Command, GitBranch,
  GitPullRequest, LayoutDashboard, ListFilter, Menu, Play, Plus, Search,
  Settings, ShieldCheck, Users, X, Zap, Clock3, CircleHelp, RefreshCw
} from "lucide-react";
import {
  Area, AreaChart, CartesianGrid, ResponsiveContainer, Tooltip,
  Bar, BarChart, XAxis, YAxis
} from "recharts";

const nav = [
  { label: "Overview", icon: LayoutDashboard },
  { label: "Issues", icon: CircleDot, count: "10" },
  { label: "Developers", icon: Users, count: "5" },
  { label: "Assignments", icon: GitPullRequest },
  { label: "Dependencies", icon: GitBranch },
  { label: "Algorithm Lab", icon: Code2 }
];

const fallbackIssues = [
  {id:101,title:"Fix Login Bug",desc:"Users cannot log in with valid credentials",skills:["Backend","Security"],priority:10,hours:3,deadline:"2026-11-12",status:"Open"},
  {id:102,title:"Improve Homepage",desc:"Redesign homepage layout for better user experience",skills:["Frontend"],priority:7,hours:4,deadline:"2026-11-14",status:"Open"},
  {id:103,title:"Fix Payment Bug",desc:"Payment fails for some users during checkout",skills:["Backend","Database"],priority:10,hours:5,deadline:"2026-11-13",status:"Open"},
  {id:104,title:"Build Dashboard",desc:"Create analytics dashboard for users",skills:["Frontend"],priority:6,hours:6,deadline:"2026-11-16",status:"Open"},
  {id:105,title:"Optimize Database",desc:"Improve database query performance",skills:["Database"],priority:8,hours:4,deadline:"2026-11-13",status:"Open"},
  {id:106,title:"Add Search Feature",desc:"Allow users to search projects",skills:["Frontend"],priority:8,hours:5,deadline:"2026-11-15",status:"Open"},
  {id:107,title:"Fix API Timeout",desc:"API requests take too long to respond",skills:["Backend"],priority:9,hours:3,deadline:"2026-11-12",status:"Open"},
  {id:108,title:"Write Unit Tests",desc:"Add automated tests for authentication",skills:["Testing"],priority:5,hours:4,deadline:"2026-11-17",status:"Open"},
  {id:109,title:"Improve Security",desc:"Fix password validation and security checks",skills:["Security"],priority:9,hours:6,deadline:"2026-11-14",status:"Open"},
  {id:110,title:"Update Documentation",desc:"Document API endpoints and usage",skills:["Documentation"],priority:4,hours:2,deadline:"2026-11-18",status:"Open"}
];
const fallbackDevelopers = [
  {id:1,name:"Aarav",hours:8,workload:0,experience:"Intermediate",skills:[{name:"Backend",level:"Advanced"},{name:"Database",level:"Intermediate"}]},
  {id:2,name:"Diya",hours:10,workload:0,experience:"Advanced",skills:[{name:"Frontend",level:"Advanced"},{name:"Testing",level:"Intermediate"}]},
  {id:3,name:"Rohan",hours:6,workload:0,experience:"Beginner",skills:[{name:"Database",level:"Intermediate"},{name:"Backend",level:"Beginner"}]},
  {id:4,name:"Ananya",hours:8,workload:0,experience:"Advanced",skills:[{name:"Backend",level:"Advanced"},{name:"Security",level:"Advanced"},{name:"Testing",level:"Advanced"}]},
  {id:5,name:"Kabir",hours:7,workload:0,experience:"Intermediate",skills:[{name:"Frontend",level:"Intermediate"},{name:"Documentation",level:"Advanced"},{name:"Testing",level:"Intermediate"}]}
];
const fallbackDeps = [{issueId:103,dependsOn:101},{issueId:104,dependsOn:102},{issueId:108,dependsOn:101},{issueId:109,dependsOn:101}];

function Avatar({name,small=false}) {
  const colors=["#8957e5","#1f9d78","#bf8700","#388bfd","#db6d28"];
  const idx=(name||"U").charCodeAt(0)%colors.length;
  return <span className={`avatar ${small?"small":""}`} style={{background:colors[idx]}}>{(name||"U").slice(0,1).toUpperCase()}</span>;
}
function Status({status}) {
  const s=(status||"Open").toLowerCase();
  return <span className={`status ${s==="assigned"||s==="closed"?"green":s==="in progress"?"purple":s==="unassigned"?"red":"blue"}`}><i/>{status||"Open"}</span>;
}
function Priority({value}) {
  const p=Number(value);
  return <span className={`priority ${p>=9?"p-high":p>=7?"p-medium":"p-low"}`}><span>{p>=9?"↑":p>=7?"↗":"↓"}</span> {p}</span>;
}
function Card({children,className=""}) { return <section className={`panel ${className}`}>{children}</section>; }

export default function App() {
  const [page,setPage]=useState("Overview");
  const [issues,setIssues]=useState(fallbackIssues);
  const [developers,setDevelopers]=useState(fallbackDevelopers);
  const [dependencies,setDependencies]=useState(fallbackDeps);
  const [assignments,setAssignments]=useState([]);
  const [search,setSearch]=useState("");
  const [filter,setFilter]=useState("All");
  const [loading,setLoading]=useState(false);
  const [message,setMessage]=useState("");
  const [report,setReport]=useState("");
  const [mobileOpen,setMobileOpen]=useState(false);

  async function loadData() {
    try {
      const r=await fetch("/api/data");
      if(!r.ok) throw new Error("API not ready");
      const d=await r.json();
      setIssues(d.issues?.length?d.issues:fallbackIssues);
      setDevelopers(d.developers?.length?d.developers:fallbackDevelopers);
      setDependencies(d.dependencies?.length?d.dependencies:fallbackDeps);
      setMessage("Connected to local project data");
    } catch {
      setMessage("Preview mode · start the local API to connect your CSV data");
    }
  }
  useEffect(()=>{loadData();},[]);

  async function runEngine() {
    setLoading(true); setMessage("");
    try {
      const r=await fetch("/api/run",{method:"POST"});
      const d=await r.json();
      if(!r.ok) throw new Error(d.error||"Could not run C++ engine");
      setAssignments(d.assignments||[]);
      setReport(d.output||"");
      setMessage("C++ assignment engine completed successfully");
      setPage("Assignments");
    } catch(e) { setMessage(e.message); }
    finally { setLoading(false); }
  }

  const assigned=assignments.filter(a=>a.developerId!==-1);
  const assignedHours=assigned.reduce((n,a)=>n+Number(a.hours||0),0);
  const unassigned=assignments.filter(a=>a.developerId===-1).length;
  const visibleIssues=useMemo(()=>issues.filter(i=>{
    const q=search.toLowerCase();
    const match=[i.title,String(i.id),...(i.skills||[])].join(" ").toLowerCase().includes(q);
    return match&&(filter==="All"||i.status===filter);
  }),[issues,search,filter]);

  const workload=developers.map(d=>{
    const allocated=assigned.filter(a=>a.developerId===d.id).reduce((n,a)=>n+Number(a.hours||0),0);
    return {...d,allocated,remaining:Math.max(0,d.hours-d.workload-allocated)};
  });

  function renderOverview() {
    const chart=[{name:"Mon",assigned:2,open:7},{name:"Tue",assigned:3,open:6},{name:"Wed",assigned:4,open:5},{name:"Thu",assigned:5,open:5},{name:"Fri",assigned:8,open:2},{name:"Sat",assigned:8,open:2},{name:"Sun",assigned:assigned.length,open:Math.max(0,issues.length-assigned.length)}];
    return <>
      <div className="welcome-row"><div><div className="eyebrow">WORKSPACE / OVERVIEW</div><h1>Good to see you again <span className="wave">✦</span></h1><p>Here’s what’s happening across your issue triage workspace.</p></div><button className="primary-btn" onClick={runEngine} disabled={loading}><Play size={15}/>{loading?"Running engine…":"Run assignment engine"}</button></div>
      <div className="stats-grid">
        <Stat label="Total issues" value={issues.length} note="Across this workspace" icon={CircleDot} tone="blue"/>
        <Stat label="Assigned" value={assignments.length?assigned.length:"—"} note={assignments.length?`${Math.round(assigned.length/issues.length*100)}% assignment rate`:"Run engine to calculate"} icon={CheckCircle2} tone="green"/>
        <Stat label="Unassigned" value={assignments.length?unassigned:"—"} note="Need attention" icon={AlertCircle} tone="orange"/>
        <Stat label="Developer capacity" value={`${developers.reduce((n,d)=>n+d.hours,0)}h`} note={`${developers.length} contributors`} icon={Users} tone="purple"/>
      </div>
      <div className="content-grid overview-grid">
        <Card><div className="panel-head"><div><h3>Issue activity</h3><p>Assignment progress over the week</p></div><span className="subtle-pill">Last 7 days</span></div><div className="chart-wrap"><ResponsiveContainer width="100%" height="100%"><AreaChart data={chart}><defs><linearGradient id="assignedFill" x1="0" y1="0" x2="0" y2="1"><stop offset="0%" stopColor="#3fb950" stopOpacity={.22}/><stop offset="100%" stopColor="#3fb950" stopOpacity={0}/></linearGradient></defs><CartesianGrid stroke="#30363d" vertical={false}/><XAxis dataKey="name" stroke="#8b949e" tickLine={false} axisLine={false}/><YAxis stroke="#8b949e" tickLine={false} axisLine={false}/><Tooltip contentStyle={{background:"#161b22",border:"1px solid #30363d",borderRadius:10}}/><Area type="monotone" dataKey="assigned" stroke="#3fb950" fill="url(#assignedFill)" strokeWidth={2.5}/><Area type="monotone" dataKey="open" stroke="#388bfd" fill="transparent" strokeWidth={2}/></AreaChart></ResponsiveContainer></div><div className="legend"><span><i className="dot green-dot"/> Assigned</span><span><i className="dot blue-dot"/> Open</span></div></Card>
        <Card><div className="panel-head"><div><h3>Developer capacity</h3><p>Hours allocated after latest run</p></div><button className="text-btn" onClick={()=>setPage("Developers")}>View all <ArrowUpRight size={13}/></button></div><div className="capacity-list">{workload.map(d=><div className="capacity-item" key={d.id}><div className="capacity-top"><div className="person"><Avatar name={d.name} small/><span>{d.name}</span></div><span className="muted">{d.allocated}/{d.hours}h</span></div><div className="progress"><span style={{width:`${Math.min(100,(d.allocated/Math.max(1,d.hours))*100)}%`}}/></div></div>)}</div></Card>
      </div>
      <Card className="table-panel"><div className="panel-head"><div><h3>Recently updated issues</h3><p>Your workspace’s latest issue list</p></div><button className="text-btn" onClick={()=>setPage("Issues")}>View all issues <ArrowUpRight size={13}/></button></div><IssueTable issues={issues.slice(0,5)} assignments={assignments}/></Card>
    </>;
  }
  function renderIssues() {
    return <><PageHeading eyebrow="WORKSPACE / ISSUES" title="Issues" subtitle="Track, prioritize and manage work across your project." action={<button className="outline-btn" onClick={()=>setMessage("Issue editing is not connected yet. Update your CSV files for now.")}><Plus size={15}/> New issue</button>}/>
      <Card className="table-panel"><div className="toolbar"><div className="searchbox"><Search size={15}/><input value={search} onChange={e=>setSearch(e.target.value)} placeholder="Search issues by title, ID or skill…"/></div><select value={filter} onChange={e=>setFilter(e.target.value)}><option>All</option><option>Open</option><option>In Progress</option><option>Closed</option></select><span className="result-count">{visibleIssues.length} results</span></div><IssueTable issues={visibleIssues} assignments={assignments}/></Card>
    </>;
  }
  function renderDevelopers() {
    return <><PageHeading eyebrow="WORKSPACE / PEOPLE" title="Developers" subtitle="Your contributors, their skills and available capacity."/><div className="developer-grid">{workload.map(d=><Card key={d.id} className="developer-card"><div className="dev-card-top"><Avatar name={d.name}/><span className="subtle-pill">{d.experience}</span></div><h3>{d.name}</h3><p className="muted">Developer #{d.id}</p><div className="skill-tags">{d.skills.map(s=><span key={s.name}>{s.name}<b>{s.level}</b></span>)}</div><div className="dev-capacity"><div><span>Allocated</span><b>{d.allocated}h / {d.hours}h</b></div><div className="progress"><span style={{width:`${Math.min(100,d.allocated/Math.max(1,d.hours)*100)}%`}}/></div><small>{d.remaining} hours remaining</small></div></Card>)}</div></>;
  }
  function renderAssignments() {
    return <><PageHeading eyebrow="WORKSPACE / PLANNING" title="Assignments" subtitle="See how the C++ engine distributes issues across your developers." action={<button className="primary-btn" onClick={runEngine} disabled={loading}><RefreshCw size={14}/>{loading?"Running…":"Run again"}</button>}/>
      {!assignments.length?<Card className="empty-state"><div className="empty-icon"><GitPullRequest size={25}/></div><h3>No assignment run yet</h3><p>Run the C++ engine to generate developer assignments and explanations from your project data.</p><button className="primary-btn" onClick={runEngine}><Play size={14}/> Run assignment engine</button></Card>:<>
        <div className="stats-grid three"><Stat label="Assigned issues" value={assigned.length} icon={CheckCircle2} tone="green"/><Stat label="Unassigned issues" value={unassigned} icon={AlertCircle} tone="orange"/><Stat label="Allocated hours" value={`${assignedHours}h`} icon={Clock3} tone="blue"/></div>
        <Card className="table-panel"><div className="panel-head"><div><h3>Assignment decisions</h3><p>Each decision includes the engine’s explanation.</p></div><span className="subtle-pill success-pill">C++ engine output</span></div><div className="assignment-list">{assignments.map(a=>{const issue=issues.find(i=>i.id===a.issueId);const dev=developers.find(d=>d.id===a.developerId);return <div className="assignment-row" key={a.issueId}><div className="assignment-main"><div className="issue-id">#{a.issueId}</div><div><b>{issue?.title||`Issue ${a.issueId}`}</b><p>{a.reason}</p></div></div><div className="assignment-owner">{dev?<><Avatar name={dev.name} small/><span>{dev.name}</span></>:<span className="muted">No match</span>}</div><Status status={a.status}/><span className="hours">{a.hours}h</span></div>})}</div></Card>
      </>}
      {report&&<details className="raw-report"><summary>View complete C++ terminal output</summary><pre>{report}</pre></details>}
    </>;
  }
  function renderDependencies() {
    return <><PageHeading eyebrow="WORKSPACE / GRAPH" title="Dependencies" subtitle="Understand which issues must be completed before others can begin."/><Card className="graph-panel"><div className="panel-head"><div><h3>Dependency map</h3><p>Arrow direction: prerequisite → dependent issue</p></div><span className="subtle-pill">{dependencies.length} connections</span></div><div className="dependency-map"><div className="graph-column">{issues.slice(0,5).map(i=><div className={`graph-node ${i.id===101?"highlight":""}`} key={i.id}><span>#{i.id}</span>{i.title}</div>)}</div><div className="graph-links"><GitBranch size={32}/><span>{dependencies.length} linked relationships</span></div><div className="graph-column">{issues.slice(5).map(i=><div className="graph-node" key={i.id}><span>#{i.id}</span>{i.title}</div>)}</div></div><div className="dependency-list">{dependencies.map(d=><div key={`${d.issueId}-${d.dependsOn}`}><span>#{d.dependsOn}</span><ArrowUpRight size={14}/><b>#{d.issueId}</b><small>{issues.find(i=>i.id===d.issueId)?.title}</small></div>)}</div></Card></>;
  }
  function renderLab() {
    const algos=[["Dependency validation","Binary search","Checks whether referenced issue IDs exist."],["Cycle detection","DFS","Detects circular dependencies in the issue graph."],["Execution order","Topological sort","Produces an order that respects prerequisites."],["Issue prioritization","Greedy strategy","Ranks currently available issues by priority and tie-breakers."],["Developer matching","Greedy assignment","Matches required skills with proficiency and capacity."],["Resource planning","0/1 Knapsack","Selects a high-value set of issues within a time budget."],["Shortest paths","Dijkstra / Bellman-Ford","Demonstrates weighted graph path algorithms."],["Network optimization","Prim / Kruskal / Flow","Demonstrates spanning trees and maximum flow."]];
    return <><PageHeading eyebrow="ENGINEERING / ALGORITHM LAB" title="Algorithm Lab" subtitle="A transparent look at the algorithms powering your triage engine."/><div className="algo-grid">{algos.map((a,i)=><Card className="algo-card" key={a[0]}><div className="algo-index">0{i+1}</div><h3>{a[0]}</h3><span className="algo-chip">{a[1]}</span><p>{a[2]}</p><span className="algo-state"><CheckCircle2 size={13}/> Implemented in C++</span></Card>)}</div></>;
  }

  return <div className="app-shell">
    <aside className={`sidebar ${mobileOpen?"mobile-open":""}`}><div className="brand"><div className="brand-mark"><GitBranch size={20}/></div><div><b>triage<span>.</span></b><small>ISSUE INTELLIGENCE</small></div><button className="mobile-close" onClick={()=>setMobileOpen(false)}><X size={18}/></button></div>
      <div className="repo-switch"><div className="repo-icon">OT</div><div><b>open-source-issue-triage</b><small>Workspace</small></div><ChevronDown size={14}/></div>
      <div className="nav-label">WORKSPACE</div><nav>{nav.map(item=><button key={item.label} className={`nav-item ${page===item.label?"active":""}`} onClick={()=>{setPage(item.label);setMobileOpen(false)}}><item.icon size={16}/><span>{item.label}</span>{item.count&&<small>{item.count}</small>}</button>)}</nav>
      <div className="sidebar-bottom"><div className="nav-label">PROJECT STATUS</div><div className="repo-health"><span className="health-dot"/><div><b>Engine ready</b><small>Local C++ workspace</small></div></div><div className="sidebar-links"><button onClick={()=>setMessage("Settings are coming soon.")}><Settings size={15}/> Settings</button><button onClick={()=>setMessage("Help documentation will be added in the README.")}><CircleHelp size={15}/> Help & docs</button></div><div className="profile"><Avatar name="Khushi"/><div><b>Workspace owner</b><small>Local developer</small></div><ChevronDown size={14}/></div></div>
    </aside>
    <main className="main-area"><header className="topbar"><button className="hamburger" onClick={()=>setMobileOpen(true)}><Menu size={19}/></button><div className="breadcrumbs"><span>open-source-issue-triage</span><b>/</b><strong>{page.toLowerCase()}</strong></div><div className="top-actions"><div className="global-search"><Search size={14}/><input placeholder="Search workspace… ( / )" value={search} onChange={e=>{setSearch(e.target.value);if(page!=="Issues")setPage("Issues")}}/><kbd>/</kbd></div><button className="icon-btn" title="Notifications"><Bell size={16}/></button><Avatar name="Khushi" small/></div></header>
      <div className="page-content">{page==="Overview"?renderOverview():page==="Issues"?renderIssues():page==="Developers"?renderDevelopers():page==="Assignments"?renderAssignments():page==="Dependencies"?renderDependencies():renderLab()}
      {message&&<div className="toast" onClick={()=>setMessage("")}><Check size={14}/>{message}<X size={14}/></div>}
      </div>
      <footer><span>© 2026 Triage · Built for better engineering workflows</span><span><span className="health-dot"/> All systems operational</span></footer>
    </main>
  </div>;
}

function Stat({label,value,note,icon:Icon,tone="blue"}) {
  return <Card className="stat-card"><div className={`stat-icon ${tone}`}><Icon size={17}/></div><div className="stat-label">{label}</div><div className="stat-value">{value}</div>{note&&<div className="stat-note">{note}</div>}</Card>;
}
function PageHeading({eyebrow,title,subtitle,action}) {
  return <div className="page-heading"><div><div className="eyebrow">{eyebrow}</div><h1>{title}</h1><p>{subtitle}</p></div>{action}</div>;
}
function IssueTable({issues,assignments=[]}) {
  return <div className="table-scroll"><table><thead><tr><th>ISSUE</th><th>STATUS</th><th>PRIORITY</th><th>REQUIRED SKILLS</th><th>ASSIGNEE</th><th>ESTIMATE</th></tr></thead><tbody>{issues.map(i=>{const a=assignments.find(x=>x.issueId===i.id);return <tr key={i.id}><td><div className="issue-title"><span className="issue-id">#{i.id}</span><b>{i.title}</b></div><small className="description">{i.desc}</small></td><td><Status status={a?.status||i.status}/></td><td><Priority value={i.priority}/></td><td><div className="skill-tags compact">{(i.skills||[]).map(s=><span key={s}>{s}</span>)}</div></td><td>{a&&a.developerId!==-1?<span className="person"><Avatar name={a.developerName} small/>{a.developerName}</span>:<span className="muted">—</span>}</td><td className="hours">{i.hours}h</td></tr>})}</tbody></table>{!issues.length&&<div className="no-results">No issues match your search.</div>}</div>;
}