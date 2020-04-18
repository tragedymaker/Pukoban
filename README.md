# Theory of computer games/2019 HW1 
## Pukoban
利用雙向BFS搜尋sokoban的變形遊戲"Pukoban"
並測試助教提供的三種測資 tiny slall medium

### Symbol 
|       | move    | push    | pull     | 
| ----- | :----:  | :-----: | :------: |
| up    | u       | U       | ^        |
| down  | d       | D       | v        |
| left  | l       | L       | <        |
| right | r       | R       | >        |

### Legends of Map
\# wall  
@ player on non-goal  
\+ player on goal  
$ box on non-goal  
\* box on goal  
. goal  
\- nongoal  

## Compile
make

## How to execute
### My solver
./pukobansolver [testdata] [outputdata]
to solve the testdata and write the answer in outputdata
### Verifer the answer
./verifier -i [testdata] -o [outputdata]
to check whether outputdata solve testdata
### Play yourself
./pukoban -i [testdata]

