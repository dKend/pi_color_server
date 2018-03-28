import socket

sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
s.connect("/tmp/pcs")
