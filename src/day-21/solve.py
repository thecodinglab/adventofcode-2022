import os
import sympy as sp

x = sp.symbols('x')

sample_eq_txt = os.popen('src/day-21/solution.b.bin src/day-21/sample.txt').read()
exec(f'sample_eq = sp.Eq({sample_eq_txt}, 0)')
sample_sol = sp.solve(sample_eq)

print(f'sample = {sample_sol}')

input_eq_txt = os.popen('src/day-21/solution.b.bin src/day-21/input.txt').read()
exec(f'input_eq = sp.Eq({input_eq_txt}, 0)')
input_sol = sp.solve(input_eq)

print(f'input = {input_sol}')
