// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
package main

import (
	"os"
	"fmt"
	"math"
	"math/rand"
	"strconv"
	"time"
)

type Point struct{ x, y float64 }

func (p Point) X() float64 {
	return p.x
}

func (p Point) Y() float64 {
	return p.y
}

func generateRandomCoord() float64 {
	random_coordinate := (-100) + rand.Float64()*(100-(-100))
	return random_coordinate
}


// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(p.X()-q.X(), p.Y()-q.Y())
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(p.X()-q.X(), p.Y()-q.Y())
}

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	var n int = len(path)
	sum := 0.0
	for i := range path {
		sum += path[i].Distance(path[(i+1)%n])
	}
	return sum
}

func getOrientation(p1, p2, p3 Point) int {
	var val float64 = (p2.Y()-p1.Y())*(p3.X()-p2.X()) - (p2.X()-p1.X())*(p3.Y()-p2.Y())

	if val > 0 { // ClockWise orientation
		return 1
	} else if val < 0 { // CounterClockWise orientation
		return 2
	} else { // Co-linear
		return 0
	}
}


func compareTo(p0, p1, p2 *Point) bool {
	var orientation int = getOrientation(*p0, *p1, *p2)

	if orientation == 0 {
		return Distance(*p0, *p2) < Distance(*p0, *p1) 
	}

	return orientation%2 == 1
}

func quicksort(path Path, p0 *Point) Path {
	if len(path) < 2 {
		return path
	}

	left, right := 0, len(path)-1

	// Pick a pivot
	pivotIndex := rand.Int() % len(path)

	// Move the pivot to the right
	path[pivotIndex], path[right] = path[right], path[pivotIndex]

	// Pile elements smaller than the pivot on the left
	for i := range path {
		if compareTo(p0, &path[i], &path[right]) {
			path[i], path[left] = path[left], path[i] // swap
			left++
		}
	}

	// Place the pivot after the last smaller element
	path[left], path[right] = path[right], path[left]

	// Go down the rabbit hole
	quicksort(path[:left], p0)
	quicksort(path[left+1:], p0)

	return path
}

func getFarthestPoint(path Path, n int) Point { // es el punto mas alejadado en (-X) y (-Y)
	var y_min float64 = path[0].Y()
	var farthest_index int = 0

	for i := 1; i < n; i++ {
		var x, y float64 = path[i].X(), path[i].Y()
		if (y < y_min) || (y_min == y && x < path[farthest_index].X()) {
			y_min = y
			farthest_index = i
		}
	}
	return path[farthest_index]
}

func generateClosedPath(path Path, n int) {
	var p0 Point = getFarthestPoint(path, n)
	quicksort(path, &p0)
}

func printPath(path Path, n int) {
	fmt.Printf("- Generating a [%d] sides figure\n", n)

	fmt.Println("- Figure's vertices")
	for i := 0; i < n; i++ {
		fmt.Printf("  - (  %.2f,  %.2f)\n", path[i].X(), path[i].Y())
	}

	fmt.Println("- Figure's Perimeter")
	fmt.Printf(" -")
	for i := 0; i < n-1; i++ {
		fmt.Printf("%f + ", path[i].Distance(path[i+1]))
	}
	fmt.Printf("%f", path[n-1].Distance(path[0]))
	// Perimeter
	fmt.Printf(" = %f\n", path.Distance())
}

func main() {
	var args []string = os.Args[1:]

	if len(args) != 1 {
		fmt.Println("Invalid input.")
		fmt.Println("Correct usage: 'go run geometry.go <number_of_sides>'.")
		return
	}

	sides, _ := strconv.Atoi(args[0])

	if sides < 3 {
		fmt.Println("Invalid input.")
		fmt.Println("The number of sides to form a path should be greater o equal than 3.")
		return
	}

	rand.Seed(time.Now().UnixNano())

	// Generate random points
	var path Path = make(Path, sides)
	for i := 0; i < sides; i++ {
		path[i] = Point{x: generateRandomCoord(), y: generateRandomCoord()}
	}

	generateClosedPath(path, sides)
	printPath(path, sides)
}

