To download a given object, URL is passed as command line argument while running the
prog ram.
In the downloa
der program, URL is parsed and domain name, path , filename a nd file
extension is fetched.
Using getaddrinfo() function , hostname is resolved to IP Address.
Then TCP Socket is created and HTTP request is sent.
HTTP Response is stored in a buffer using recv() function.
HTTP Response Header is removed and the payload is stored in file using the Buffer.

To download
the entire website A text file all the linked object is passed as command line
argument while running the program
Each URL from the text fi
le is fetched and is downloaded using loop.
In the downloader program, URL is parsed and domain name, path, filename and file
extension is fetched.
Using getaddrinfo() function, hostn
ame is resolved to IP Address.
Then, TCP Socket is created and HTTP request is sent.
HTTP Response is stored in a buffer using recv() function.
HTTP Response Header is removed and the payload is stored in file using the Buffer.
Once the file is stored, next
URL is fetched from the text file and the same TCP Socket is
used for downloading the file.
