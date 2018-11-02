import sys

def convertir_para_c(nombre_test):
    nombre_file_c = str(nombre_test)+".txt"
    nombre_test += ".vrp"
    try:
        fin = open(nombre_test,"r")
    except IOError:
        print ("No se encuentra el archivo: ", nombre_test)
        sys.exit()

    fout = open(nombre_file_c, "w")

    for value in list(fin.readlines()):

        if("DIMENSION" in value):
            ar = []
            ar+=value
            dim = []
            for v in ar:
                if(ord(v)>=48 and ord(v)<=57):
                    dim += v
        if("CAPACITY" in value):
            ar = []
            ar+=value
            cap = []
            for v in ar:
                if(ord(v)>=48 and ord(v)<=57):
                    cap += v
    dimen = ""
    capacidad = ""
    dimen = dimen.join(dim)
    capacidad = capacidad.join(cap)
    fin.close()

    fin = open("X/"+nombre_test,"r")
    #Escribo en archivo de salida la capacida y la DIMENSION
    fout.write(str(dimen)+" " + capacidad+"\n")

    enc_node_sec = 0
    cant_nod_met = 0
    enc_demand_sec = 0
    cant_demand_met = 0
    for val in list(fin.readlines()):
        if(enc_node_sec==1 and cant_nod_met<int(dimen)):
            fout.write(str(val))
            cant_nod_met +=1
        if("NODE_COORD_SECTION" in val):
            enc_node_sec = 1
        if(enc_demand_sec==1 and cant_demand_met < int(dimen)):
            fout.write(str(val))
            cant_demand_met+=1
        if("DEMAND_SECTION" in val):
            enc_demand_sec = 1

    fin.close()
    fout.close()
    print("Archivo " , nombre_file_c , " generado correctamente")

def main():
    print ("Corriendo ", sys.argv[0])
    if(len(sys.argv) < 2 or len(sys.argv) >2):
        print("Error, faltan o sobran parametros")
    else:
        nombre = str(sys.argv[1])
        convertir_para_c(nombre)


main()


#Podria plantear lo siguiente:
#Me fijo el promedio de las demandas y en función de eso determino cual deberia ser el vecindarioself.
#Puedo chequear antes si existe algun vecindario de esa cantidad de nodos que supere la demandaself.
