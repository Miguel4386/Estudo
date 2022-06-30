from ctypes.wintypes import HDC


H1= int(input("Informe a hora inicial:"))
M1= int(input("Informe o minuto inicial:"))
H2= int(input("Informe a hora de término:"))
M2= int(input("Informe o minuto de término:"))

Ms= M2-M1
Hs= H2-H1

if Ms==0 | Hs==0:#24h
    Hd= 24
    Md= 0

elif Ms<=70:#min negativo
    Md1= (Hs*60)+Ms
    Hd= Md1//60
    Md= Md1%60
else:
    Hd=Hs
    Md=Hs
     

print("O jogo durou {0}hora(s) e {1}minuto(s)". format(Hd, Md))