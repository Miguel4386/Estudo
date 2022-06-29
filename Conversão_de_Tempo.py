t1= int(input("Imforme o tempo em segundo:"))

if t1>=3600:#horas
    Th= t1//3600
    Tm1= t1%3600
    Tm= Tm1//60
    Ts= Tm1%60

elif t1<3600:#minutos
    Th= 0
    Tm1= t1%3600
    Tm= Tm1//60
    Ts= Tm1%60

elif 0<t1<60:#segundos
    Th= 0
    Tm= 0
    Ts=t1

print("O tempo de duração do evento é: {0}h : {1}m : {2}s". format(Th, Tm, Ts))