# DC SPLP and p-median problem solver:

Compile programs:
```bash
make all
```
Create `100` experiment templates for $n \in {100,200,300,400,500}$:
```bash
bash generate_templates.sh "100 200 300 400 500" 100
```
answer `y` to delete previous problems and results.

Solve them using all the algorithms:
```bash
bash solve_problems.sh
```

Merge results:
```bash
bash merge_results.sh
```

Generate summaries:
```bash
python3 tools/graph_gen.py
```
- Answer `"results"`.
- Answer `1`.
- Answer `100`.
- Press any key.
