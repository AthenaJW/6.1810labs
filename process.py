
with open("save_register_command.txt") as f:
	for line in f:
		el = line.split(",")
		register = el[0].split(" ")[-1]
		offset = el[1].split("(")[0]
		print("t->"+register + " = old_t->" + register + ";")
		#old_t->ra = *(a0 + 10);
