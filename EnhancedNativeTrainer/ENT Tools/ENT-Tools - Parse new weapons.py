import xml.etree.ElementTree as ET

# XML_input1 = """"""

outFile = open('newWeapons.txt', 'w')
root = ET.parse('weapons.xml')

# root = ET.fromstring(XML_input1)
weaponItems = root.find('weaponShopItems')

for weapon in weaponItems.findall('Item'):
    name = weapon.find('nameHash').text
    print(name + ":")
    outFile.write(name + ":" + "\n\n")
    weaponComponents = weapon.find('weaponComponents')
    #outFile.write(name + ":" + "\n\n")
    print("\t", end="")

    for weaponComponent in weaponComponents.findall('Item'):
        print("\"" + weaponComponent.find('componentName').text + "\"" + ", ", end="")
        outFile.write("\"" + weaponComponent.find('componentName').text + "\"" + ", ")
    print("\n")

input("Press Enter to continue...")
