# CSE/ECE 6730

The Google Doc for this readme is available [here](https://docs.google.com/document/d/13ZdCe5m2qWxUTOIV5w832NaWi0wwD3mAR7vmARB5Kj8/edit?usp=sharing).

This readme is formatted using Github-flavored Markdown. Learn more [here](https://guides.github.com/features/mastering-markdown/).

## Meeting Notes

Next meeting is **2017/4/29** 19:00.

### 2017/4/23 20:00

### Project 3

- [@bma36](https://github.gatech.edu/bma36):
    - Code to write out trajectories to HDF5
    - Integration code to string together reading input, running simulation, and dumping output
    - Will implement at least LJ force computation.  If time permits, then also Bond/Angle/Dihedral and electrostatic force computation
    - Finalize the config file / parameters
- [@cseng3](https://github.gatech.edu/cseng3): Use a Topology / Force Field database (Force Field Parameters file, Parameters file, etc.) to convert a pdb (just bond information) file into a topology file (a list enumerating all bond, angle, dihedral, non-bond parameters) and a trajectory file (a list enumerating the (initial) atom positions)
- [@hsi9](https://github.gatech.edu/hsi9) and [@fxiong32](https://github.gatech.edu/fxiong32): Trajectory energy calculation and visualization
    - Run random_topology.py to generate random_topology.h5
    - Look inside random_topology.h5 with the hdfview program, figure out the structure. If you need help with the structure, the python script shows how the file was created. Other examples in C++ are in links on the Slack and Google Hangouts history.
    - Create a script in whatever language that calculates and outputs the energy of each particle.
    - Perform a visualization of the output using any visualizer.
- [@ssugimoto3](https://github.gatech.edu/ssugimoto3): Force calculator for bonds, dihedrals (proper torsions), improper torsions

### 2017/4/21

[Project 2](https://www.dropbox.com/s/2nmxho8xgt71p57/cse6730.zip?dl=0) turned in. Dropbox link will be alive until [@ssugimoto3](https://github.gatech.edu/ssugimoto3) finds it no longer relevant and deletes it.

### 2017/4/9, 2017/4/16

Project 2

---

#### Project 2

Project due 2017/4/21. **All code due 2017/4/18**.

- Non-distributed: Just logging left
- Trace-viewer: gl;hf

#### Project 3

Project due 2017/4/30? ~~**All code due 2017/4/25**~~ **REMOVED: Unrealistic deadline**.

- Proposal: Writing and turning it in 2017/4/10 (Assigned [@ssugimoto3](https://github.gatech.edu/ssugimoto3))
- Division of labor: *First come first serve*. Take your pick.

---

### 2017/4/2 20:00

Meeting postponed.

---

### 2017/3/26 20:30

### Project 2

Progress report:

- [@bma36](https://github.gatech.edu/bma36): Almost done with YAML configuration; partway through HDF5 reading.
- [@cseng3](https://github.gatech.edu/cseng3): Confirmed build; started writing code.
- [@hsi9](https://github.gatech.edu/hsi9): learning scala.
- [@ssugimoto3](https://github.gatech.edu/ssugimoto3): YAML, HDF5 Schema draft on Slack. Slow but non-zero progress on Scala. Going to a hackathon next weekend, so will have significant progress by then. (Side effect: Probably missing next week’s meeting.)
- [@fxiong32](https://github.gatech.edu/fxiong32): Just reading and learning.

Changes to assignments:

- Reassigning non-distributed to [@hsi9](https://github.gatech.edu/hsi9) and [@fxiong32](https://github.gatech.edu/fxiong32). Notification around 2017/3/31 to [@ssugimoto3](https://github.gatech.edu/ssugimoto3) if dropping it due to workload.
- Adding [@ssugimoto3](https://github.gatech.edu/ssugimoto3) to Trace-Viewer if help requested by 2017/3/31.
- **Moving code writing due date to 2017/4/7.** Meeting on 2017/4/2 will be progress report 2, with possible discussion on starting the report and video presentation. Meeting on 2017/4/9 will touch on code review if necessary and will focus on report writing and video presentation creation.

---

### 2017/3/19 20:20

#### Project 2

We have 3 meetings left before the due date (2017/4/7 + DL 1 week = 2017/4/14), so start early! We need the last meeting to talk about things like data-collection runs, report writing, and video presentation creation (yes that is a thing, look on T-Square), so it’d be best to think of the implementations as due in 2.5 weeks, i.e. ~~2017/4/4~~ 2017/4/7. The remaining 10 days will be used for code review and non-code parts.

Work has been assigned as follows:

- Non-distributed
    - Assign: [@ssugimoto3](https://github.gatech.edu/ssugimoto3)
- Null Message
    - Assign: [@ssugimoto3](https://github.gatech.edu/ssugimoto3)
- YAWNS
    - Assign: [@cseng3](https://github.gatech.edu/cseng3)
- Trace-Viewer
    - Figure out how Catapult works, specify output format (Assign: [@fxiong32](https://github.gatech.edu/fxiong32), [@hsi9](https://github.gatech.edu/hsi9))
- HDF5
    - PR [#9](https://github.gatech.edu/bma36/cse6730/pull/9) (Assign: [@bma36](https://github.gatech.edu/bma36))
    - Maybe make object-based bindings? Low priority (Assign: [@bma36](https://github.gatech.edu/bma36))
    - Figure out schema for airplane table and airport table (Assign: [@ssugimoto3](https://github.gatech.edu/ssugimoto3)). ~~Due 2017/3/21.~~ Done 2017/3/22.
- YAML
    - PR [#9](https://github.gatech.edu/bma36/cse6730/pull/9) (Assign: [@bma36](https://github.gatech.edu/bma36))
    - Figure out configuration parameters that we’re going to use (Assign: [@ssugimoto3](https://github.gatech.edu/ssugimoto3)). ~~Due 2017/3/20.~~ Done 2017/3/21.

---

### 2017/3/12 20:00

#### Project 2

Feature 3: HDF5. Proposal’s Done!

---

### 2017/3/5 17:00

#### Project 2

Project 2 is **high priority**! Proposal write-up is due **2017/3/17**. Draft is [here](https://www.overleaf.com/8412863mmhnzsdbxvct). Things we have to do are:

- Non-distributed option (required)
- YAWNS distributed option (required)
- Null Message distributed option (required)
- YAML configuration feature
- After the fact visualization ([Trace-viewer](https://github.com/catapult-project/catapult/tree/master/tracing))
- 3rd feature is TBD

Notes on how we’ll work:

- Try to work off of Scala / Java and MPI ([Java Bindings for MPI](http://blogs.cisco.com/performance/java-bindings-for-open-mpi)), if not then fall back on C++
- Proposal submission window on T-Square will be reopened for *both on-campus and distance learning* near deadline; we will submit in said window.

#### Project 3

Project 3 is **low priority**!

- Check Up: How everyone is faring with HDF5 and the textbook (a.k.a. How bored everyone has become with the content): Slow progress, but quite irrelevant as long as the project works out
- Take a look at the C++ code in the repository, try to figure out its use and importance: Deferred in favor of project 2 discussion

---

### 2017/2/26 20:00

Low attendance, so moving check up to next week (3/5). Things to do have not changed (check last week’s to-do list). Noticed that Chapter 3 is around 70 pages; don’t burn yourself out trying to read all of it!

A little more clarification on molecular dynamics simulations: main inputs are the topology file and configuration file.

Topology File: Snapshot of the system (all major atoms in the simulation) at t=0. The process of **Topology Building** consists of:

- Read topology file
- Initialize velocities?
- Add hydrogen atoms (not listed in structure)
- Add water
- Add ions (charge counterbalance)
- (Copy structure as necessary)

Configuration File: Runtime parameters (We’ll be using [YAML](http://www.yaml.org/). The v1.2 spec’s “Chapter 2. Preview” has examples of yaml files.). Configuration Building is applying the parameters to the simulation.

- Temperature
- Box size
- Integration method
- Forces to include

HDF5 links:

- Parallel/Intermediate HDF5: https://www.nersc.gov/assets/NUG-Meetings/HDF5-tutorialNUG2010.pdf
- Basic HDF5: http://www.math-cs.gordon.edu/courses/cps343/presentations/HDF5.pdf
- Older HDF5 Tutorial: http://web.mit.edu/fwtools_v3.1.0/www/H5.intro.html
- Example HDF5-like format for MD: http://ambermd.org/netcdf/nctraj.pdf
- HDF5 vs NetCDF: http://stackoverflow.com/questions/1075074/opinions-on-netcdf-vs-hdf5-for-storing-scientific-data

HDF5 Things to know:

- Datatypes
- Datasets
- Attributes

C++ Things to know:
- Good coding practice when using C++: http://www.protein.osaka-u.ac.jp/rcsfp/pi/omegagene/docs/html/dev-manual/cpp-language-guidance.html

---

### 2017/2/19 21:00

#### **Things to do** for next week:

- Think of a project name (long-form and three-letter abbrev.)
- Read Chapters 1, 3 of the textbook as necessary (download link in last week’s notes)
- Read HDF5 tutorials as necessary (website available [here](https://support.hdfgroup.org/HDF5/))
- Learn git as necessary (#learn-git)

First steps after HDF5 tutorial: Get the pre-run code working

1. Figure out how HDF5 (the “file system in a file”) works
2. Figure out what we need from HDF5 (File system paths, Datasets, etc.)
3. Build the topology builder with HDF5

#### Molecular Dynamics 101

##### Methodology

At its core, the molecular dynamics we will be doing has 2 steps:

1. Figure out the forces on each particle.
2. Use the forces on each particle to figure out the velocity and position in the next time step.

A much more detailed explanation as a PNG image is [here](http://www.gromacs.org/@api/deki/files/230/=gmx_4.6_gpu_acceleration.png).

- Decompose space into neighbor lists (i.e. figure out which particles are nearby)
- Bond energies/forces (Part of step 1.)
- Non-bond energies/forces (Part of step 1.)
- PME: Electrostatics (Part of step 1.)
- Integration: Force vectors to velocity, position (Step 2.) - see http://www.slideshare.net/keerthanpg/integration-schemes-in-molecular-dynamics

##### Toolset

A PDF explaining the tools commonly used in MD is [here](http://research.ozreef.org/GROMACS_MD_Flowchart.pdf).

- Topology: positions, bonds, angles, dihedrals. “What the simulated object looks like”
- Trajectory: positions, velocities, forces “Where the particles in the simulated object are going”

#### Long-term Workload

Things we have to build:

- Pre-run topology builder (Python?)
    - We should use a standard file format for topology (e.g. hdf5: Portable binary “file system in a file” file format. Python, C++ libraries available) -> Added to things to do
- Simulation engine (C++)
- Post-run analysis (C++, Java, Python?)
    - Visualization

What we already have (will be added to the Github soon-ish):

- Integration
- Neighbor search
- PME (Chapter 7 of the textbook)

What we don’t already have:

- MPI Domain Decomposition (split simulation box into smaller cubes for parallel processing)
- Some other simulation engine parts?
- Pre-run and Post-run code

---

### 2017/2/12 20:00

#### Communication

We have concluded on using the following mediums:

- **Slack** for announcements / file transfers / git notifications / non-live discussions
- **Google Hangouts** for live discussions during meetings

#### Git and Github

We will be using the [Georgia Tech Github](https://github.gatech.edu/) server, which allows for unlimited private repositories. **The repository is located [here](https://github.gatech.edu/bma36/cse6730).** You must first log in with your Georgia Tech credentials (which should be the same as T-Square).

There’s a git tutorial available [here](http://rogerdudler.github.io/git-guide/). This and more resources are available under #learn-git on Slack.

#### Project topics

- Location tracking for hospital equipment
- Box model(?)
- **Protein structure simulation**
    - “Numerical Simulation in Molecular Dynamics” - Chapter 3 is the best place for start for code examples. **Book can be downloaded [here](http://link.springer.com/book/10.1007%2F978-3-540-68095-6).**

#### Languages

We’ve agreed on using **C++** for performance critical parts, with possibly other languages for supplemental programs and scripts (Java, Scala, Python, etc.)

#### Workload

Deferred to next week.
