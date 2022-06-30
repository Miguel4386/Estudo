c1= int(input("Informe o código do produto 1:"))
c2= int(input("Informe o código do produto 2:"))
q1= int(input("Informe a quantidade de itens do produto 1:"))
q2= int(input("Informe a quantidade de itens do produto 2:"))
v1= float(input("Informe o valor do produto 1:"))
v2= float(input("Informe o valor do produto 2:"))

VF= float((v1*q1)+(v2*q2))

print("Valor a Pagar: R$ %.2f" %(VF))