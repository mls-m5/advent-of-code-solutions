
def fuelRecursive(mass):
	fuel = max(0, int(mass / 3) - 2)
	if fuel > 0:
		return fuel + fuelRecursive(fuel)
	else:
		return fuel
	
with open("../data/1.txt") as file:
	mass = [int(line) for line in file]
fuel1 = sum([int(m / 3) -2 for m in mass])
fuel2 = sum([fuelRecursive(m) for m in mass])

print ("answer 1: {}".format(fuel1))
print ("answer 2: {}".format(fuel2))


