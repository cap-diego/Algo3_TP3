import networkx as nx
import numpy as np
import csv
from numpy import loadtxt
import matplotlib.pyplot as plt
import pylab
import sys

cnames = {
#'black':                '#000000', lo saco para usar de deposito
'blanchedalmond':       '#FFEBCD',
'blue':                 '#0000FF',
'blueviolet':           '#8A2BE2',
'brown':                '#A52A2A',
'burlywood':            '#DEB887',
'cadetblue':            '#5F9EA0',
'chartreuse':           '#7FFF00',
'chocolate':            '#D2691E',
'coral':                '#FF7F50',
'cornflowerblue':       '#6495ED',
'cornsilk':             '#FFF8DC',
'crimson':              '#DC143C',
'cyan':                 '#00FFFF',
'darkblue':             '#00008B',
'darkcyan':             '#008B8B',
'darkgoldenrod':        '#B8860B',
'darkgray':             '#A9A9A9',
'darkgreen':            '#006400',
'darkkhaki':            '#BDB76B',
'darkmagenta':          '#8B008B',
'darkolivegreen':       '#556B2F',
'darkorange':           '#FF8C00',
'darkorchid':           '#9932CC',
'darkred':              '#8B0000',
'darksalmon':           '#E9967A',
'darkseagreen':         '#8FBC8F',
'darkslateblue':        '#483D8B',
'darkslategray':        '#2F4F4F',
'darkturquoise':        '#00CED1',
'darkviolet':           '#9400D3',
'deeppink':             '#FF1493',
'deepskyblue':          '#00BFFF',
'dimgray':              '#696969',
'dodgerblue':           '#1E90FF',
'firebrick':            '#B22222',
'floralwhite':          '#FFFAF0',
'forestgreen':          '#228B22',
'fuchsia':              '#FF00FF',
'gainsboro':            '#DCDCDC',
'ghostwhite':           '#F8F8FF',
'gold':                 '#FFD700',
'goldenrod':            '#DAA520',
'gray':                 '#808080',
'green':                '#008000',
'greenyellow':          '#ADFF2F',
'honeydew':             '#F0FFF0',
'hotpink':              '#FF69B4',
'indianred':            '#CD5C5C',
'indigo':               '#4B0082',
'lime':                 '#00FF00',
'limegreen':            '#32CD32',
'linen':                '#FAF0E6',
'magenta':              '#FF00FF',
'maroon':               '#800000',
'mediumaquamarine':     '#66CDAA',
'mediumblue':           '#0000CD',
'mediumorchid':         '#BA55D3',
'mediumpurple':         '#9370DB',
'mediumseagreen':       '#3CB371',
'mediumslateblue':      '#7B68EE',
'mediumspringgreen':    '#00FA9A',
'mediumturquoise':      '#48D1CC',
'mediumvioletred':      '#C71585',
'midnightblue':         '#191970',
'mintcream':            '#F5FFFA',
'mistyrose':            '#FFE4E1',
'moccasin':             '#FFE4B5',
'navajowhite':          '#FFDEAD',
'navy':                 '#000080',
'oldlace':              '#FDF5E6',
'olive':                '#808000',
'olivedrab':            '#6B8E23',
'orange':               '#FFA500',
'orangered':            '#FF4500',
'orchid':               '#DA70D6',
'palegoldenrod':        '#EEE8AA',
'palegreen':            '#98FB98',
'paleturquoise':        '#AFEEEE',
'palevioletred':        '#DB7093',
'papayawhip':           '#FFEFD5',
'peachpuff':            '#FFDAB9',
'peru':                 '#CD853F',
'pink':                 '#FFC0CB',
'plum':                 '#DDA0DD',
'powderblue':           '#B0E0E6',
'purple':               '#800080',
'red':                  '#FF0000',
'rosybrown':            '#BC8F8F',
'royalblue':            '#4169E1',
'saddlebrown':          '#8B4513',
'salmon':               '#FA8072',
'sandybrown':           '#FAA460',
'seagreen':             '#2E8B57',
'seashell':             '#FFF5EE',
'sienna':               '#A0522D',
'silver':               '#C0C0C0',
'skyblue':              '#87CEEB',
'slateblue':            '#6A5ACD',
'slategray':            '#708090',
'snow':                 '#FFFAFA',
'springgreen':          '#00FF7F',
'steelblue':            '#4682B4',
'tan':                  '#D2B48C',
'teal':                 '#008080',
'thistle':              '#D8BFD8',
'tomato':               '#FF6347',
'turquoise':            '#40E0D0',
'violet':               '#EE82EE',
'wheat':                '#F5DEB3',
'white':                '#FFFFFF',
'whitesmoke':           '#F5F5F5',
'yellow':               '#FFFF00',
'yellowgreen':          '#9ACD32'}

def graficarGrafo(nombre,nombre_completo):
    G = nx.Graph()
    path = "salida/"+nombre +"/"+nombre_completo +".csv"
    f = plt.figure(nombre)
    with open(path) as csvfile:
        spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
        for row in spamreader:
            if(len(row) == 3):
                G.add_node(int(row[0]),pos=(float(row[1]),float(row[2])))
            elif(len(row) == 2):
                G.add_edge(int(row[0]),int(row[1]))
            else:
                costo_tour = row[0]
            pos=nx.get_node_attributes(G,'pos')
    nx.draw_networkx(G,pos, node_size=40)


def graficarNodos(nombre):
    G = nx.Graph()
    path = "salida/"+ nombre +"/"+ nombre + ".csv"
    f = plt.figure(nombre)
    lista = []
    fixed_positions = {}
    dic_colores = {}
    with open(path) as csvfile:
        spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
        for row in spamreader:
            fixed_positions[row[0]] = (float(row[1]),float(row[2]) )
            dic_colores[row[3]] = str(row[3])
            G.add_node(row[0], cluster=row[3], pos=(row[1],row[2]))
    fixed_nodes = fixed_positions.keys()

    #necesito lista de g.nodes.size pposiciones con los dic_colores
    colores = []
    listacnames=[]
    #print(len(list(G.nodes())))
    for i in cnames.keys() :
        listacnames += [cnames[i]]
    largo = int(len(listacnames))
    #print(largo)
    #   print(list(G.nodes()))
    print("\n\n")
    #print(list(G.nodes(data=True)))
    for i in G.nodes(data='cluster'):
        #print("Nodo: " + i[0] + " cluster: " + i[1] )

        colores += [listacnames[(int(dic_colores[i[1]])) % largo] ]
        if(int(i[0]) == 1 ): # si es el deposito
            colores[(len(colores)-1)] = '#000000'
        #colores += [listacnames[(int(dic_colores[(G.node[str(i)]['cluster'])] ) ) % largo]]

    #colores[0] = '#000000'
    pos = nx.spring_layout(G,pos=fixed_positions, fixed = fixed_nodes)
    nx.draw_networkx(G,pos,node_color=colores,node_size=65, with_labels=True)

    print("Cantidad de clusters" , len(dic_colores)-1)


def main():

    print ("Corriendo ", sys.argv[0])
    if(len(sys.argv) < 2 or len(sys.argv) >2):
        print("Error, faltan o sobran parametros")
    else:
        nombre = str(sys.argv[1])
        nombre_archivo = nombre
        #nombre_archivo = "M-n101-k10"
        graficarNodos(nombre_archivo)
        plt.show()
        graficarGrafo(nombre_archivo, nombre_archivo+"-ruteo0")
        graficarGrafo(nombre_archivo, nombre_archivo+"-ruteo1")
        graficarGrafo(nombre_archivo, nombre_archivo+"-ruteo2")
        graficarGrafo(nombre_archivo, nombre_archivo+"-ruteo3")
        graficarGrafo(nombre_archivo, nombre_archivo+"-ruteo4")
        graficarGrafo(nombre_archivo, nombre_archivo+"-ruteo5")

        plt.show()

main()
