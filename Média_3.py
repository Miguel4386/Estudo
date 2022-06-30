N1= float(input("Informe a primeira nota:"))
N2= float(input("Informe a segunda nota:"))
N3= float(input("Informe a terceira nota:"))
N4= float(input("Informe a quarta nota:"))

#Média
Np1= N1*0.2
Np2= N2*0.3
Np3= N3*0.4
Np4= N4*0.1

MT= Np1+Np2+Np3+Np4

print("Média: %.1f" %(MT))

if MT>=7.0:#Situação
    print("Aluno aprovado")

elif MT<5.0:
    print("Aluno reprovado")

elif 5.0<MT<6.9:
    print("Aluno em exame")
    Ne= float(input("Informe a nota do exame:"))
    MTr= (MT+Ne)/2
    if MTr>=5.0:
        print("Aluno aprovado")
        print("Média final: %.1f" %(MTr))
    elif MTr<5.0:
        print("aluno reprovado")
        print("Média final: %.1f" %(MTr))