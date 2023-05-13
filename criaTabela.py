import os
from os.path import isfile, join
import sys

mainPath = "C:/Users/radsi/Graduacao/implementacoes"
instancesFolderPath = "C:/Users/radsi/Graduacao/implementacoes/instancias"
extensionFile = ".max"
testeName = "PRv3_2023_04_21"

instancesNames = [f for f in os.listdir(instancesFolderPath) if isfile(join(instancesFolderPath, f))]
instancesNames = [instance for instance in instancesNames if extensionFile in instance]

def levalor(texto, arquivo):
  pin = arquivo.find(texto)
  if pin == -1:
    return ""
  pfin = arquivo.find('\n',pin)
  tamanho = len(texto)
  return  arquivo[pin+tamanho:pfin]

method = testeName.split('_')[0]
nomeArquivoDados = testeName + "/" + method + ".csv"
arquivoDados = open(nomeArquivoDados, "w")

print("Nome da instancia,fluxo,tempo(ns),vertices,arcos,caminhos")
arquivoDados.write("Nome da instancia,fluxo,tempo(ns),vertices,arcos,caminhos\n")

for i in instancesNames:
  nomeArquivoDeSolucao = testeName + "/" + i + ".log"
  arquivoDeSolucao = open(nomeArquivoDeSolucao, "r")
  solucao = arquivoDeSolucao.read()
  fluxo = levalor("fluxo: ", solucao)
  tempo = levalor("tempo: ", solucao)
  vertices = levalor("vertices: ", solucao)
  arcos = levalor("arcos: ", solucao)
  caminhos = levalor("caminhos: ", solucao)
  print(i + "," + fluxo + "," + tempo + "," + vertices + "," + arcos + "," + caminhos)
  arquivoDados.write(i + "," + fluxo + "," + tempo + "," + vertices + "," + arcos + "," + caminhos + "\n")
  arquivoDeSolucao.close()
arquivoDados.close()
