main:
	g++ -fno-stack-protector -g -Iinclude -Llib "./main.cpp" "./src/glad.c" -lglfw3 -lssp -o "./bin/main.exe"

test:
	g++ -fno-stack-protector -g -Iinclude -Llib "./test.cpp" "./src/glad.c" -lglfw3 -lssp -o "./bin/test.exe"

clean:
	rm -f *.o ./bin/main.exe ./bin/test.exe