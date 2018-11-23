package main

import (
	"fmt"
	"math"
)

const k float64 = 1.380648 * 1e-23
const e float64 = 1.9 * 1e-19

const Mc_M0 float64 = 1.18
const Mv_M0 float64 = 0.81
const M0 float64 = -1.0
const Eg float64 = 1.9 * 1e-19

//from menu
const Ed float64 = 1e4 //?
const Ea float64 = 1e6 //?
const Nd0 float64 = 1.75 * 1e16
const Nda float64 = 1.48 * 1e15
const T float64 = 300

//func calc_effective_density_state(mcv float64, m0 float64, T float64) float64 {
func calc_effective_density_state(mcv float64, T float64) float64 {
	return 2.51 * math.Pow(10, 19) * math.Pow((mcv*T/300), 1.5)
}

func calc_n(Nc float64, mu float64, Eg float64, T float64) float64 {
	return Nc * math.Exp((mu-Eg)/(k*T))
}

func calc_p(Nv float64, mu float64, Eg float64, T float64) float64 {
	return Nv * math.Exp((-mu)/(k*T))
}

func calc_Fermi_level(Nc float64, Nv float64, Nd0 float64, Ea float64, Ed float64, Eg float64, T float64) float64 {
	var c1 float64 = Nd0 - Nc
	var c2 float64 = Nd0*math.Exp(Ea/k/T) - Nc*(math.Exp(Ea/k/T)+math.Exp(Eg-Ed))
	var c3 float64 = Nv*(1+math.Exp(Ea/k/T)+math.Exp((Eg-Ed)/k/T)+math.Exp((Eg-Ed+Ea)/k/T)) - Nc*math.Exp((Eg-Ed+Ea)/k/T)
	return k * T / 3 * (math.Log(c1) + math.Log(c2) + math.Log(c3))
}

/*
find_mobility() {
	get from Tima and Dima ??
}
*/

func Find_conductivity() float64 {
	var muE float64 = 1400
	var muP float64 = 450

	var Nc float64 = calc_effective_density_state(Mc_M0, T)
	fmt.Println("Nc=", Nc)
	var Nv float64 = calc_effective_density_state(Mv_M0, T)
	fmt.Println("Nv= ", Nv)
	var Ef float64 = calc_Fermi_level(Nc, Nv, Nd0, Ea, Ed, Eg, T)
	fmt.Println("Ef=", Ef)
	var n float64 = calc_n(Nc, Ef, Eg, T)
	fmt.Println("n= ", n)
	var p float64 = calc_p(Nv, Ef, Eg, T)
	fmt.Println("p= ", p)
	return e * (n*muE + p*muP)
}
