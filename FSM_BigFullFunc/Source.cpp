#include <plog/Log.h>

#include <intrin.h> // там __rdtsc()
#include <time.h>

#include <iostream>

#pragma once

using namespace std;


bool F1(bool* x) {
	return x[0] & x[1] & ((!x[2] & x[3]) | (x[4] & !x[5])) | (!x[6] & x[7] & !x[2] & x[0]) | (!x[0] & x[5] & x[7] & !x[6]) |
		(x[7] & !x[0]) | (x[8] & x[9] & x[10]);
}

bool F2(bool* x) {
	return (x[1] & x[2] & x[3]) | (x[0] & x[4] & x[5]) | (x[6] & !x[7] & !x[1] & !x[2]) | (x[1] & !x[0] & !x[1]) | (x[9] & x[10] & x[11]);
}

bool F3(bool* x) {
	return (!x[0] & x[2] & !x[3]) | (x[4] & x[1] & x[5]) | (x[6] & x[7] & x[0] & x[1]) | (x[8] & x[9] & !x[10]) | (x[0] & x[11] & x[2] & x[3]);
}

bool F4(bool* x) {
	return x[5] & x[1] & ((!x[2] & x[3]) | (x[0] & !x[7])) | (!x[6] & x[5] & !x[7] & x[4]) | (x[8] & x[9]) | (x[10] & x[11]);
}

bool F5(bool* x) {
	return (x[0] & x[1] & x[2] & x[3] & x[4]) | (x[3] & x[4] & x[5]) | (x[6] & x[7] & x[5]) | (x[7] & x[0] & x[1]);
}

bool F6(bool* x) {
	return (!x[0] & x[1] & !x[7] & !x[5]) | (x[1] & !x[2] & x[6]) | (x[2] & !x[3] & x[7] & !x[5]) | (!x[3] & x[4] & !x[6]) | (x[4] & !x[5] & x[6])
		| (!x[5] & x[6] & !x[4]) | (x[6] & !x[7] & x[3]) | (!x[7] & x[0] & !x[2] & !x[5]) | (x[7] & x[6] & !x[1] & x[2]);
}

bool F7(bool* x) {
	return (x[1] & x[2] & x[3] & x[4] & x[5] & x[6] & x[7]) | (!x[0] & !x[1]) | (x[9] & x[10] & x[11] & x[0]) | (x[2] & !x[11] & x[5] & !x[10]) |
		(!x[8] & !x[9] & !x[10] & !x[11]);
}


__int64 hz_cpu() { // замер частоты
	clock_t t_clock;
	__int64 t_tsc;
	t_clock = clock() + CLOCKS_PER_SEC;
	t_tsc = __rdtsc(); // взять TSC
	while (clock() < t_clock); // отсчет одной секунды
	return (__rdtsc() - t_tsc); // частота в герцах
}

class FSM_1 {
public:
	bool i10 = 0, i2 = 0, i0 = 0;
	int St[6][3] = { 0, 0, 0,
	0, 0, 1,
	0, 1, 0,
	0, 1, 1,
	1, 0, 0,
	1, 0, 1 };

	bool y1 = 0, y2 = 0, y3 = 0, y4 = 0, y5 = 0;
	__int64 freq;
	FSM_1(__int64 frq) {
		freq = frq;
		LOGD << "FSM_1";
	}

	void y_calc(int i) {
		y1 = (!St[i][0] & St[i][2]) | (St[i][0] & !St[i][1] & St[i][2]);
		y2 = (!St[i][0] & St[i][2]) | (St[i][0] & !St[i][1] & !St[i][2]);
		y3 = (!St[i][0] & St[i][1] & St[i][2]) | (St[i][0] & !St[i][1] & !St[i][2]);
		y4 = (!St[i][0] & St[i][1] & !St[i][2]) | (St[i][0] & !St[i][1] & St[i][2]);
		y5 = !St[i][0] & St[i][1] & !St[i][2];
	}

	int S0(bool* x) {
		if (F2(x) & F1(x)) {
			if (!i0) {
				i0 = true;
				return 0;
			}
			else {
				return -1;
			}
		}
		else if (!F1(x) & F2(x)) {
			return 1;
		}
		else if (!F2(x)) {
			return 3;
		}
		return -1;
	}

	int S1(bool* x) {
		if (!F2(x)) {
			return 4;
		}
		else if (F2(x) & F3(x)) {
			return 2;
		}
		else if (F2(x) & !F3(x)) {
			return 5;
		}
		return -1;
	}

	int S2(bool* x) {
		if (F2(x) & F2(x) & !F3(x)) {
			return 5;
		}
		else if (F2(x) & !F2(x)) {
			return 4;
		}
		else if (!F2(x)) {
			return 3;
		}
		else if (F2(x) & F2(x) & F3(x)) {
			if (!i2) {
				i2 = true;
				return 2;
			}
			else {
				return -1;
			}
		}
		return -1;
	}

	int S3(bool* x) {
		if (F2(x) & F3(x)) {
			return 2;
		}
		else if (F2(x) & !F3(x)) {
			return 5;
		}
		else if (!F2(x)) {
			return 4;
		}
		return -1;
	}

	int S4(bool* x) {
		if (F4(x)) {
			return 8;
		}
		else if (!F4(x) & F5(x)) {
			return 9;
		}
		else if (!F4(x) & !F5(x)) {
			return 10;
		}
		return -1;
	}

	int S5(bool* x) {
		if (F4(x)) {
			return 4;
		}
		else if (!F4(x)) {
			return 6;
		}
		return -1;
	}

	int S6(bool* x) {
		if (F5(x)) {
			return 4;
		}
		else if (!F5(x)) {
			return 7;
		}
		return -1;
	}

	int S7(bool* x) {
		if (F6(x)) {
			return 4;
		}
		else if (!F6(x) & F7(x)) {
			return 11;
		}
		else if (!F6(x) & !F7(x)) {
			return 12;
		}
		return -1;
	}

	int S8(bool* x) {
		return -1;
	}

	int S9(bool* x) {
		return -1;
	}

	int S10(bool* x) {
		if (F4(x)) {
			return 8;
		}
		else if (!F4(x) & F5(x)) {
			return 9;
		}
		else if (!F4(x) & !F5(x)) {
			if (!i10) {
				i10 = true;
				return 10;
			}
			else {
				return -1;
			}
		}
		return -1;
	}

	int S11(bool* x) {
		if (F5(x)) {
			return 12;
		}
		else if (!F5(x)) {
			return -1;
		}
		return -1;
	}

	int S12(bool* x) {
		return -1;
	}

	void run(bool* x) {
		LOG(plog::debug) << F1(x) << F2(x) << F3(x) << F4(x) << F5(x) << F6(x) << F7(x);
		//	printf_s("F1,F2,F3: %d %d %d\n", F1, F2, F3);
		//	cout << x[0] << x[1] << x[2] << x[3]<<endl;
		int j = 0, i = 0;
		while (true) {
			i = j;
			y_calc(j);
			//		cout << "S" << j << "->";
			__int64 start = __rdtsc();
			switch (j) {
			case 0: {
				j = S0(x);
				break;
			}
			case 1: {
				j = S1(x);
				break;
			}
			case 2: {
				j = S2(x);
				break;
			}
			case 3: {
				j = S3(x);
				break;
			}
			case 4: {
				j = S4(x);
				break;
			}
			case 5: {
				j = S5(x);
				break;
			}
			case 6: {
				j = S6(x);
				break;
			}
			case 7: {
				j = S7(x);
				break;
			}
			case 8: {
				j = S8(x);
				break;
			}
			case 9: {
				j = S9(x);
				break;
			}
			case 10: {
				j = S10(x);
				break;
			}
			case 11: {
				j = S11(x);
				break;
			}
			case 12: {
				j = S12(x);
				break;
			}
			case -1:
				i10 = false, i2 = false, i0 = false;
				return;
			}
			//	cout << "S" << j << endl;
			LOG(plog::debug) << "S" << i << ">" << j << " " << 1000000000 * (__rdtsc() - start) / freq;
		}
	}
};

int main() {
	plog::init(plog::debug, "FSM_BigFullFunc.txt");
	__int64 freq = hz_cpu();

	int len = 4000;
	bool x_s[12];

	FSM_1 fsm(freq);
	cin >> len;
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < 12; j++) {
			cin >> x_s[j];
		}
		fsm.run(x_s);
	}
	LOG(plog::debug) << "out of run";
	return 0;

}