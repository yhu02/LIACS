int main() {
	int var_a, var_b, var_c, var_d, var_e, var_f, var_g, var_h, var_i, var_j, var_k;
	var_a = 660380129;
	var_c = 1389186834;
	var_d = 12239696;
	var_h = 828506939;
	var_i = 1057775228;
	var_f = var_c%(var_h);
	var_j = var_a%var_f;
	var_h = var_a%var_i;
	print ((-var_j)%var_d)+(-(var_a+var_d/var_j))%(var_a+(var_j));
}