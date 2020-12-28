import sys
import requests

# Minecraft uses SRV records to resolve hostnames

if len(sys.argv) < 2:
    print("Not enough arguments (provide an ip)")
else:
    ip = str(sys.argv[1])
    r = requests.get('https://dns.google/resolve?name=_minecraft._tcp.%s&type=SRV' % ip)
    if 'Answer' not in r.json():
        print("IP is " + ip)
    else:
        ans = r.json()['Answer']
        assert(len(ans)==1)
        ans = ans[0]
        priority, weight, port, addr = ans['data'].split()
        print(addr, port)
