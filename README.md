# CSE/ECE 6730

The Google Doc for this readme is available [here](https://docs.google.com/document/d/13ZdCe5m2qWxUTOIV5w832NaWi0wwD3mAR7vmARB5Kj8/edit?usp=sharing).

This readme is formatted using Github-flavored Markdown. Learn more [here](https://guides.github.com/features/mastering-markdown/).

## Meeting Notes

Next meeting is **2017/2/26** 20:00.

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
- Integration: Force vectors to velocity, position (Step 2.)

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

- **Slack** for announcements / file transfers / git notifications
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
