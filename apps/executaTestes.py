import os
from os.path import isfile, join

# Editar nome do teste, data de execução e diretório das instancias

# ["BAP", "CS", "SAP", "HPRv1", "HPRv2", "HPRv3", "HPRv4", "HPRv5", "HPRv6", "HPRv7", "HPRv8", "PR"]
method = ["BAP", "CS", "SAP", "HPRv1", "HPRv2", "HPRv3", "HPRv4", "HPRv5", "HPRv6", "HPRv7", "HPRv8", "PR"]
date = "2023_16_07"

mainPath = "/home/ricardo/Downloads/Fluxo/MaximumFlowAlgorithms"
testesFolder = "testes"
instancesFolderPath = "/home/ricardo/Downloads/Fluxo/MaximumFlowAlgorithms/instances"
extensionFile = ".in"

# Coleta as instâncias de <instancesFolderPath>
instancesNames = [f for f in os.listdir(instancesFolderPath) if isfile(join(instancesFolderPath, f))]
instancesNames = [instance for instance in instancesNames if extensionFile in instance]
instancesNames.sort()

# Cria uma pasta para armazenar os testes
if not os.path.exists(testesFolder):
  print("Criando folder " + testesFolder)
  os.makedirs(testesFolder)


for mtd in method:
  mainScript = "./bin/main $instance " + mtd
  testeName = mtd + "_" + date

  # Cria a pasta de teste para o método a ser executado
  if not os.path.exists(os.path.join(testesFolder, testeName)):
    print("Criando folder " + testeName)
    os.makedirs(os.path.join(testesFolder, testeName))

  # Executa programa para cada uma das instâncias
  for i in instancesNames:
    comando = mainScript.replace("$instance", instancesFolderPath + "/" + i)
    comando = comando + " > " + "./" + testesFolder + "/" + testeName + "/" + i + ".log"
    print(comando)
    os.system(comando)

    