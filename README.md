# HPDA Workshop Documents

# Introduction to Parallel Programming

# Table of Contents
* [Topics Covered](#topics-covered)
* [Team Members](#team-members)

# Topics to be Covered:
 * Theory
   * Example of where Parallel Programming is necessary
   * Theory of PP performance Analysis : intro to Ahmdahl's Law (Strong Scaling) and Gustafson's Law (Weak Scaling)

* Languages/Libraries/Directive that can be used OPENMP, MPI GPU(not covered)
   * Shared(OPENMP)  Memory Programming Model
     * Introduce basic example of openmp ("Hello Word")
       * Examples:
          * serial to parallel matrix multiplication
          * reduction example: Numerical Integration
          * (Scheduling example: SpMV, show performance depends on matrix structure -> scheduling)
          * (ccNUMA example: 2D heat with Jacobi, Show performance depends on the data layout when using multi-socket system)
        * Final Example: Mandelbrot Set

* Distributed (MPI) Memory Programming Model (Maybe only for advanced group ? and basic example for beginner group ? )
    * Message Passing Interface (MPI - Importance of communication
    * Introduce basic example of OpenMPI ("Hello Word")
      * Examples:
        * serial to parallel matrix multiplication
        * reduction example: Numerical Integration

      * Final Example: MandelBrot Set

# <a name="team-members"></a>Team Members
* "Hof. Moritz Travis" <moritz.hof@dlr.de>
* "Overmars. Nigel"  <nigel.overmars@dlr.de>                


created Edited: Moritz Hof, 07.10.2020
last Edited: Moritz Hof, 16.10.2020
