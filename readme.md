# Particle-Based-Fluid-Simulation

## Usage

You could compile code with `sh compile.sh 2` for generating 2-Dimension solver or `sh compile.sh 3` for 3-Dimension solver.

After modifying parameters in `tools/input.json`, execute following commands:

```
./build/main tools/input.json
python3 build/SPH_painter.py tools/input.json
```

Results would be like:

![Figure_1](.assets/Figure_1.png "Figure_1")
![Figure_2](.assets/Figure_2.png "Figure_2")

## TODO
- Dimension in config.
- Adding Animation.
