from pythonosc import dispatcher
from pythonosc import osc_server

def handler(addr,*args):
    print(addr,args)

disp = dispatcher.Dispatcher()
disp.set_default_handler(handler)

server = osc_server.ThreadingOSCUDPServer(
("0.0.0.0",9000),disp)

print("listening")
server.serve_forever()