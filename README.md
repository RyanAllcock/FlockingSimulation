~~~ Boids ~~~

~ How To Use:
	- compile with C++ alongside GLM
	- attach to simulation program, display using data, and update each loop using function

~ In This Project:
	- basic 2D Boids implementation
	- distance-proportional separation version 1 (https://www.youtube.com/watch?v=QbUPfMXXQIY)
	- close-proximity separation version 2 (https://vergenet.net/~conrad/boids/pseudocode.html or http://www.kfish.org/boids/pseudocode.html)

~ Boids:
	- Method for simulating flocking behaviours in particle bodies.
	- Used in animation, with scenes including dynamic obstacles or dense populations.
	- Provides automated, smooth and organic pathfinding for a set of bodies.
	- Yields positional and directional data for displaying entities.

~ General Algorithm of Boids: https://www.red3d.com/cwr/boids/
	- Define a set of bodies with position, velocity, and acceleration.
	- Model various steering behaviours, which affect acceleration each update.
	- Simulate each update by finding the bodies' new behaviour-affected velocities and positions, by doing the following for each body:
	- ... Numerically, increment acceleration into velocity, then increment the resulting velocity into current position.

~ Types of Boids:
	- basic (separation, alignment, cohesion)
	- wall avoiding (wall avoidance, boundary avoidance)
	- psychological (goal/fear driven)
	- specialised (vehicular, perching)

~ What to add next
	- decide parameterisation & type for stable boids beyond 100units^2 area & timestep of 1 (a matter of tweaking the model, finding good documentation, and finding good presentations)
	- 3D boids; allow setSize to be 3D seamlessly (should be independent of window borders, especially if 3D where it could be a "fish tank" arrangement), then perhaps implement glm::vec2/3 templates
	- wall repelling force
	- goal attraction force
	- obstacle avoidance (many different methods to choose from) https://www.red3d.com/cwr/nobump/nobump.html
	- wall perching condition (freeze motion sometimes when near bottom border for random perch period, then resume boid-ing behaviour)
	- randomised velocity biased along axes, and doesn't scale with motion limits, parameters/constants, etc.
	- replace cruise speed approach method, from acc-up-to-value to acc-to-value; seems to break when lerping to cruise speed using general lerp method
	- complex behavioural distance influences (e.g. cubic behaviour intensity with distance, causing more/less violent close-by interactions)
	- random boid velocity perturbations

~ More Interesting/Extra Links:
	- (boids academic explanation/introduction) http://www.cs.toronto.edu/~dt/siggraph97-course/cwr87/
	- (searching for boids algorithm information) https://www.bing.com/search?q=boids+algorithm&PC=U316&FORM=CHROMN
	- (application, etc. of boids) https://en.wikipedia.org/wiki/Boids
	- (looking for boids papers) https://scholar.google.co.uk/scholar?hl=en&as_sdt=0%2C5&q=boids&btnG=
	- (formal look at boids by academics) https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=c14378dad6d17d5f42cbf5623edc42fbd348bfb3
	- (MSc on vehicular boids) https://etheses.whiterose.ac.uk/17534/8/Aastha%20Thesis%20Fixed.pdf
	- (implementation demonstration, or boids flocking behaviours demonstration with comparison to starlings) https://eater.net/boids or https://www.youtube.com/watch?v=4LWmRuB-uNU
	- (step-by-step implementation) https://www.youtube.com/watch?v=mhjuuHl6qHM
	- (typically uninformative boids documentation) https://akhilsathuluri.github.io/tutorials/2020/02/29/boids.html
	- (common problems with implementation) https://www.bing.com/search?q=boids+stop+moving+after+a+minute&PC=U316&FORM=CHROMN or https://www.google.co.uk/search?q=my+%22boids%22+are+still&source=hp&ei=wpJ4ZPafE6eO9u8Pq_O4yAg&iflsig=AOEireoAAAAAZHig0lJbwgJA9_xLn2A7DKQebADq0aVh&ved=0ahUKEwj2iPbqjKL_AhUnh_0HHas5DokQ4dUDCAs&uact=5&oq=my+%22boids%22+are+still&gs_lcp=Cgdnd3Mtd2l6EAM6EAguEAMQjwEQ6gIQjAMQ5QI6EAgAEAMQjwEQ6gIQjAMQ5QI6BQghEKABUO8BWMAkYLMmaAFwAHgAgAGTAYgBigOSAQMwLjOYAQCgAQKgAQGwAQo&sclient=gws-wiz
	- (creator appearance at stackoverflow) https://stackoverflow.com/questions/6209829/flocking-boids-behaviour-problem
	- (fully-fledged 3d boids simulation application demonstration) https://www.youtube.com/watch?v=bqtqltqcQhw
	- (finding boids papers, hopefully can find better explanations and examples this way later in the future) https://www.bing.com/search?q=boids+paper&PC=U316&FORM=CHROMN