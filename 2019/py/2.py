

def loadNumbers(filename):
    with open(filename, "r") as file:
        strs = file.read().split(",")
        return [int(x) for x in strs]


def runProgram(numbers, noun, verb):
    finished = False
    index = 0
    numbers[1] = noun
    numbers[2] = verb
    while not finished:
        command = numbers[index]
        input1 = numbers[numbers[index + 1]]
        input2 = numbers[numbers[index + 2]]
        if command == 1:
            numbers[numbers[index + 3]] = input1 + input2
        elif command == 2:
            numbers[numbers[index + 3]] = input1 * input2
        elif command == 99:
            finished = True
            break;
        else:
            raise RuntimeError("error")
            break
        
        index += 4

    return numbers[0];



numbers = loadNumbers("../data/2.txt");

    
print("answer 1: {}".format(runProgram(numbers[:], 12, 2)))

for noun in range(100):
    for verb in range(100):
        if 19690720 == runProgram(numbers[:], noun, verb):
            print("noun: {} verb: {}".format(noun, verb))
            print("answer 2: {}".format(noun * 100 + verb))
            exit()



