#include <stdio.h>
#include <stdint.h>

int building_age_years(int current_year, int built_year, int months_since_anniv)
{
    if(current_year < built_year)
        return 0;
    int years = current_year - built_year;
    int age_months = years * 12 + months_since_anniv;
    return age_months / 12;
}


int main(void) {
    int current = 2025, built = 1990, months = 0;
    int age = building_age_years(current, built, months);
    printf("Building age: %d years\n", age);
    return 0;
}

