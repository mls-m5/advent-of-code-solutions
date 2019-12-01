
package main

import "fmt"
import "os"
import "bufio"
import "strconv"

func fuelRecursive(mass int, recurse bool) int {
	fuel := int(mass / 3) - 2
	if fuel > 0 && recurse {
		return fuel + fuelRecursive(fuel, true)
	} else {
		return fuel
	}
}

func readNums(filename string) []int {
	file, _:= os.Open(filename)

	var nums []int

	scanner := bufio.NewScanner(file)

	for scanner.Scan() {
		num, _ := strconv.Atoi(scanner.Text())
		nums = append(nums, num)
	}

	return nums

}

func main() {
	nums := readNums("../data/1.txt")

	{
		sum := 0

		for _, num := range nums {
			sum += fuelRecursive(num, false)
		}


		fmt.Println("answer 1: ", sum)
	}

	{
		sum := 0
		for _, num := range nums {
			sum += fuelRecursive(num, true)
		}


		fmt.Println("answer 2: ", sum)
	}
}