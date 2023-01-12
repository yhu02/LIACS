int main() {
	int var_a, var_b;
	var_a = 169816216;
	var_b = 1418536546;
	var_a = (-var_b)%var_a*var_a%var_b/var_a;
	var_a = var_b*(var_a)/var_b;
	var_a = var_b+var_b;
	print var_a;
}