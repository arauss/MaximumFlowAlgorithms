import os
from os.path import isfile, join
import sys

# Compilar programa principal antes de rodar
# Editar nome do teste e data

method = ["PRv3", "PRv4"]
date = "2023_04_21"


for mtd in method:
  mainScript = "a.exe $instance " + mtd
  testeName = mtd + "_" + date

  mainPath = "C:/Users/radsi/Graduacao/implementacoes"
  instancesFolderPath = "C:/Users/radsi/Graduacao/implementacoes/instancias"
  extensionFile = ".max"

  instancesNames = [f for f in os.listdir(instancesFolderPath) if isfile(join(instancesFolderPath, f))]
  instancesNames = [instance for instance in instancesNames if extensionFile in instance]

  if not os.path.exists(testeName):
    print("Criando folder " + testeName)
    os.makedirs(testeName)

  for i in instancesNames:
    comando = mainScript.replace("$instance", instancesFolderPath + "/" + i)
    comando = comando + " > " + "./" + testeName + "/" + i + ".log"
    print(comando)
    os.system(comando)