i=0

while [ $i -lt 100 ]	
	do
		./fileget.py -f fsp://file.server.two/folder_down/file2.txt -n 127.0.0.2:42069
		i=$((i+1))
	done


