import os
from os.path import isfile, join
import sys


mainPath = "/home/ricardo/Downloads/Fluxo/MaximumFlowAlgorithms"
instancesFolderPath = "/home/ricardo/Downloads/Fluxo/MaximumFlowAlgorithms/instances"
testesFolderPath = "/home/ricardo/Downloads/Fluxo/MaximumFlowAlgorithms/testes"
extensionFile = ".max"
testeName = "HPRvblist_2023_16_07"

instancesNames = [f for f in os.listdir(instancesFolderPath) if isfile(join(instancesFolderPath, f))]
instancesNames = [instance for instance in instancesNames if extensionFile in instance]
instancesNames.sort()

method = testeName.split('_')[0]
nomeArquivoDados = testesFolderPath + "/" + testeName + "/" + method + ".csv"
arquivoDados = open(nomeArquivoDados, "w")

print("Nome da instancia,Arcos,Vertices,Fluxo,Tempo(ns),Pushs,Relabels")
arquivoDados.write("Nome da instancia,Arcos,Vertices,Fluxo,Tempo(ns),Pushs,Relabels\n")

for i in instancesNames:
  nomeArquivoDeSolucao = testesFolderPath + "/" + testeName + "/" + i + ".log"
  arquivoDeSolucao = open(nomeArquivoDeSolucao, "r")
  solucao = arquivoDeSolucao.readlines()
  instance_name = i.split(".")[0]
  data = [instance_name] + [line.strip().split(": ")[1] for line in solucao if ":" in line]
  print(",".join(data))
  arquivoDados.write(",".join(data) + "\n")
arquivoDados.close()