5.13. Напишите программу, которая через аргументы командной строки получает имена и аргументы 
для запуска внешних программ (произвольное их количество), разделённые параметром, состоящим из двух символов «;»; 
чтобы при запуске командный интерпретатор не считал параметр «;;» имеющим особый смысл, заключайте его в апострофы, например:
    ./prog ls -l / ';;' sleep 10 ';;' cat file1.txt file2.txt
Ваша программа должна запустить на одновременное (параллельное) исполнение все указанные программы с заданными аргументами 
и напечатать имена тех из них, которые завершились успешно, то есть вызовом _exit с параметром 0. 
Печатать имена следует по мере завершения запущенных программ. 
Закончить работу следует сразу после завершения последней из запущенных программ, но не раньше.
