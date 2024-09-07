main:
	g++ -fno-stack-protector -g -Iinclude -Llib "./main.cpp" "./src/glad.c" -lglfw3 -lssp -o "./main.exe"