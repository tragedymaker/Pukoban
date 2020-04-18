# Theory of computer games/2019 HW1 
## Pukoban
利用雙向BFS搜尋sokoban的變形遊戲"Pukoban"
並測試助教提供的三種測資

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
##
./pukobansolver [testdata] [outputdata]
