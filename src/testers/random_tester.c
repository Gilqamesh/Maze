#include "../console/console.h"
#include "../random/random.h"

#define NUMBER_OF_BUCKETS 1000

static void u64_tester(struct console* console, struct random* random_generator, u32* buckets, u32 bucket_size) {
    memset(buckets, 0, bucket_size * sizeof(*buckets));

    u64 total_cys_to_generate_random_numbers = 0;
    u64 min_u64 = 1564124654;
    u64 max_u64 = 16446744073709551615ULL;

    console__log(console, "  --== u64 Test ==--  \n");
    u32 max_random_number_counter = 10000000;
    for (u32 random_number_counter = 0; random_number_counter < max_random_number_counter; ++random_number_counter) {
        u64 start_time = __rdtsc();
        u64 cur_random_number = random__u64_closed(random_generator, min_u64, max_u64);
        total_cys_to_generate_random_numbers += __rdtsc() - start_time;
        if (cur_random_number < min_u64 || cur_random_number > max_u64) {
            console__fatal(console, "unexpected u64 value %u in 'u64_tester'\n", cur_random_number);
        }
        u32 bucket_index = (u32) (((r64) cur_random_number - (r64) min_u64) / ((r64) max_u64 - (r64) min_u64) * (r64) bucket_size);
        ++buckets[bucket_index];
    }

    r64 chi_square_test_statistic = 0.0;
    for (u32 bucket_index = 0; bucket_index < bucket_size; ++bucket_index) {
        r64 expected_frequency = (r64) max_random_number_counter / (r64) bucket_size;
        r64 observed_frequency = (r64) buckets[bucket_index];
        r64 current_dispersion = pow((observed_frequency - expected_frequency), 2.0) / expected_frequency;
        chi_square_test_statistic += current_dispersion;
    }

    console__log(console, "chi_square_test_statistic for u64: %lf\n", chi_square_test_statistic);
    console__log(console, "total_cys_to_generate_random_numbers for %d u64 numbers: %u\n", max_random_number_counter, total_cys_to_generate_random_numbers);
    console__log(console, "cy for generating 1 u64 number: %lf\n", (r64) total_cys_to_generate_random_numbers / (r64) max_random_number_counter);
}

static void u32_tester(struct console* console, struct random* random_generator, u32* buckets, u32 bucket_size) {
    memset(buckets, 0, bucket_size * sizeof(*buckets));

    u64 total_cys_to_generate_random_numbers = 0;
    u32 min_u32 = 123;
    u32 max_u32 = 3546815346;

    console__log(console, "  --== u32 Test ==--  \n");
    u32 max_random_number_counter = 10000000;
    for (u32 random_number_counter = 0; random_number_counter < max_random_number_counter; ++random_number_counter) {
        u64 start_time = __rdtsc();
        u32 cur_random_number = random__u32_closed(random_generator, min_u32, max_u32);
        total_cys_to_generate_random_numbers += __rdtsc() - start_time;
        if (cur_random_number < min_u32 || cur_random_number > max_u32) {
            console__fatal(console, "unexpected u32 value %u in 'u32_tester'\n", cur_random_number);
        }
        u32 bucket_index = (u32) (((r64) cur_random_number - (r64) min_u32) / ((r64) max_u32 - (r64) min_u32) * (r64) bucket_size);
        ++buckets[bucket_index];
    }

    r64 chi_square_test_statistic = 0.0;
    for (u32 bucket_index = 0; bucket_index < bucket_size; ++bucket_index) {
        r64 expected_frequency = (r64) max_random_number_counter / (r64) bucket_size;
        r64 observed_frequency = (r64) buckets[bucket_index];
        r64 current_dispersion = pow((observed_frequency - expected_frequency), 2.0) / expected_frequency;
        chi_square_test_statistic += current_dispersion;
    }

    console__log(console, "chi_square_test_statistic for u32: %lf\n", chi_square_test_statistic);
    console__log(console, "total_cys_to_generate_random_numbers for %d u32 numbers: %u\n", max_random_number_counter, total_cys_to_generate_random_numbers);
    console__log(console, "cy for generating 1 u32 number: %lf\n", (r64) total_cys_to_generate_random_numbers / (r64) max_random_number_counter);
}

static void u16_tester(struct console* console, struct random* random_generator, u32* buckets, u32 bucket_size) {
    memset(buckets, 0, bucket_size * sizeof(*buckets));

    u64 total_cys_to_generate_random_numbers = 0;
    u16 min_u16 = 123;
    u16 max_u16 = 60000;

    console__log(console, "  --== u16 Test ==--  \n");
    u32 max_random_number_counter = 10000000;
    for (u32 random_number_counter = 0; random_number_counter < max_random_number_counter; ++random_number_counter) {
        u64 start_time = __rdtsc();
        u16 cur_random_number = random__u16_closed(random_generator, min_u16, max_u16);
        total_cys_to_generate_random_numbers += __rdtsc() - start_time;
        if (cur_random_number < min_u16 || cur_random_number > max_u16) {
            console__fatal(console, "unexpected u16 value %u in 'u16_tester'\n", cur_random_number);
        }
        u32 bucket_index = (u32) (((r64) cur_random_number - (r64) min_u16) / ((r64) max_u16 - (r64) min_u16) * (r64) bucket_size);
        ++buckets[bucket_index];
    }

    r64 chi_square_test_statistic = 0.0;
    for (u32 bucket_index = 0; bucket_index < bucket_size; ++bucket_index) {
        r64 expected_frequency = (r64) max_random_number_counter / (r64) bucket_size;
        r64 observed_frequency = (r64) buckets[bucket_index];
        r64 current_dispersion = pow((observed_frequency - expected_frequency), 2.0) / expected_frequency;
        chi_square_test_statistic += current_dispersion;
    }

    console__log(console, "chi_square_test_statistic for u16: %lf\n", chi_square_test_statistic);
    console__log(console, "total_cys_to_generate_random_numbers for %d u16 numbers: %u\n", max_random_number_counter, total_cys_to_generate_random_numbers);
    console__log(console, "cy for generating 1 u16 number: %lf\n", (r64) total_cys_to_generate_random_numbers / (r64) max_random_number_counter);
}

static void i64_tester(struct console* console, struct random* random_generator, u32* buckets, u32 bucket_size) {
    memset(buckets, 0, bucket_size * sizeof(*buckets));

    u64 total_cys_to_generate_random_numbers = 0;
    i64 min_i64 = -5416541454455436;
    i64 max_i64 = 89465142252654341;

    console__log(console, "  --== i64 Test ==--  \n");
    u32 max_random_number_counter = 10000000;
    for (u32 random_number_counter = 0; random_number_counter < max_random_number_counter; ++random_number_counter) {
        u64 start_time = __rdtsc();
        i64 cur_random_number = random__i64_closed(random_generator, min_i64, max_i64);
        total_cys_to_generate_random_numbers += __rdtsc() - start_time;
        if (cur_random_number < min_i64 || cur_random_number > max_i64) {
            console__fatal(console, "unexpected i64 value %u in 'i64_tester'\n", cur_random_number);
        }
        u32 bucket_index = (u32) (((r64) cur_random_number - (r64) min_i64) / ((r64) max_i64 - (r64) min_i64) * (r64) bucket_size);
        ++buckets[bucket_index];
    }

    r64 chi_square_test_statistic = 0.0;
    for (u32 bucket_index = 0; bucket_index < bucket_size; ++bucket_index) {
        r64 expected_frequency = (r64) max_random_number_counter / (r64) bucket_size;
        r64 observed_frequency = (r64) buckets[bucket_index];
        r64 current_dispersion = pow((observed_frequency - expected_frequency), 2.0) / expected_frequency;
        chi_square_test_statistic += current_dispersion;
    }

    console__log(console, "chi_square_test_statistic for i64: %lf\n", chi_square_test_statistic);
    console__log(console, "total_cys_to_generate_random_numbers for %d i64 numbers: %u\n", max_random_number_counter, total_cys_to_generate_random_numbers);
    console__log(console, "cy for generating 1 i64 number: %lf\n", (r64) total_cys_to_generate_random_numbers / (r64) max_random_number_counter);
}
static void i32_tester(struct console* console, struct random* random_generator, u32* buckets, u32 bucket_size) {
    memset(buckets, 0, bucket_size * sizeof(*buckets));

    u64 total_cys_to_generate_random_numbers = 0;
    i32 min_i32 = -543275223;
    i32 max_i32 = 325875784;

    console__log(console, "  --== i32 Test ==--  \n");
    u32 max_random_number_counter = 10000000;
    for (u32 random_number_counter = 0; random_number_counter < max_random_number_counter; ++random_number_counter) {
        u64 start_time = __rdtsc();
        i32 cur_random_number = random__i32_closed(random_generator, min_i32, max_i32);
        total_cys_to_generate_random_numbers += __rdtsc() - start_time;
        if (cur_random_number < min_i32 || cur_random_number > max_i32) {
            console__fatal(console, "unexpected i32 value %u in 'i32_tester'\n", cur_random_number);
        }
        u32 bucket_index = (u32) (((r64) cur_random_number - (r64) min_i32) / ((r64) max_i32 - (r64) min_i32) * (r64) bucket_size);
        ++buckets[bucket_index];
    }

    r64 chi_square_test_statistic = 0.0;
    for (u32 bucket_index = 0; bucket_index < bucket_size; ++bucket_index) {
        r64 expected_frequency = (r64) max_random_number_counter / (r64) bucket_size;
        r64 observed_frequency = (r64) buckets[bucket_index];
        r64 current_dispersion = pow((observed_frequency - expected_frequency), 2.0) / expected_frequency;
        chi_square_test_statistic += current_dispersion;
    }

    console__log(console, "chi_square_test_statistic for i32: %lf\n", chi_square_test_statistic);
    console__log(console, "total_cys_to_generate_random_numbers for %d i32 numbers: %u\n", max_random_number_counter, total_cys_to_generate_random_numbers);
    console__log(console, "cy for generating 1 i32 number: %lf\n", (r64) total_cys_to_generate_random_numbers / (r64) max_random_number_counter);
}
static void i16_tester(struct console* console, struct random* random_generator, u32* buckets, u32 bucket_size) {
    memset(buckets, 0, bucket_size * sizeof(*buckets));

    u64 total_cys_to_generate_random_numbers = 0;
    i16 min_i16 = -16656;
    i16 max_i16 = 22000;

    console__log(console, "  --== i16 Test ==--  \n");
    u32 max_random_number_counter = 10000000;
    for (u32 random_number_counter = 0; random_number_counter < max_random_number_counter; ++random_number_counter) {
        u64 start_time = __rdtsc();
        i16 cur_random_number = random__i16_closed(random_generator, min_i16, max_i16);
        total_cys_to_generate_random_numbers += __rdtsc() - start_time;
        if (cur_random_number < min_i16 || cur_random_number > max_i16) {
            console__fatal(console, "unexpected i16 value %u in 'i16_tester'\n", cur_random_number);
        }
        u32 bucket_index = (u32) (((r64) cur_random_number - (r64) min_i16) / ((r64) max_i16 - (r64) min_i16) * (r64) bucket_size);
        ++buckets[bucket_index];
    }

    r64 chi_square_test_statistic = 0.0;
    for (u32 bucket_index = 0; bucket_index < bucket_size; ++bucket_index) {
        r64 expected_frequency = (r64) max_random_number_counter / (r64) bucket_size;
        r64 observed_frequency = (r64) buckets[bucket_index];
        r64 current_dispersion = pow((observed_frequency - expected_frequency), 2.0) / expected_frequency;
        chi_square_test_statistic += current_dispersion;
    }

    console__log(console, "chi_square_test_statistic for i16: %lf\n", chi_square_test_statistic);
    console__log(console, "total_cys_to_generate_random_numbers for %d i16 numbers: %u\n", max_random_number_counter, total_cys_to_generate_random_numbers);
    console__log(console, "cy for generating 1 i16 number: %lf\n", (r64) total_cys_to_generate_random_numbers / (r64) max_random_number_counter);
}

static void r64_tester(struct console* console, struct random* random_generator, u32* buckets, u32 bucket_size) {
    memset(buckets, 0, bucket_size * sizeof(*buckets));

    u64 total_cys_to_generate_random_numbers = 0;
    r64 min_r64 = -100000000.0;
    r64 max_r64 = -min_r64;

    console__log(console, "  --== r64 Test ==--  \n");
    u32 max_random_number_counter = 10000000;
    for (u32 random_number_counter = 0; random_number_counter < max_random_number_counter; ++random_number_counter) {
        u64 start_time = __rdtsc();
        r64 cur_random_number = random__r64_closed(random_generator, min_r64, max_r64);
        total_cys_to_generate_random_numbers += __rdtsc() - start_time;
        if (cur_random_number < min_r64 || cur_random_number > max_r64) {
            console__fatal(console, "unexpected r64 value %lf in 'r64_tester'\n", cur_random_number);
        }
        u32 bucket_index = (u32) (((r64) cur_random_number - (r64) min_r64) / ((r64) max_r64 - (r64) min_r64) * (r64) bucket_size);
        ++buckets[bucket_index];
    }

    r64 chi_square_test_statistic = 0.0;
    for (u32 bucket_index = 0; bucket_index < bucket_size; ++bucket_index) {
        r64 expected_frequency = (r64) max_random_number_counter / (r64) bucket_size;
        r64 observed_frequency = (r64) buckets[bucket_index];
        r64 current_dispersion = pow((observed_frequency - expected_frequency), 2.0) / expected_frequency;
        chi_square_test_statistic += current_dispersion;
    }

    console__log(console, "chi_square_test_statistic for r64: %lf\n", chi_square_test_statistic);
    console__log(console, "total_cys_to_generate_random_numbers for %d r64 numbers: %u\n", max_random_number_counter, total_cys_to_generate_random_numbers);
    console__log(console, "cy for generating 1 r64 number: %lf\n", (r64) total_cys_to_generate_random_numbers / (r64) max_random_number_counter);
}

static void r32_tester(struct console* console, struct random* random_generator, u32* buckets, u32 bucket_size) {
    memset(buckets, 0, bucket_size * sizeof(*buckets));

    u64 total_cys_to_generate_random_numbers = 0;
    r32 min_r32 = -100000.0f;
    r32 max_r32 = -min_r32;

    console__log(console, "  --== r32 Test ==--  \n");
    u32 max_random_number_counter = 10000000;
    for (u32 random_number_counter = 0; random_number_counter < max_random_number_counter; ++random_number_counter) {
        u64 start_time = __rdtsc();
        r32 cur_random_number = random__r32_closed(random_generator, min_r32, max_r32);
        total_cys_to_generate_random_numbers += __rdtsc() - start_time;
        if (cur_random_number < min_r32 || cur_random_number > max_r32) {
            console__fatal(console, "unexpected r32 value %f in 'r32_tester'\n", cur_random_number);
        }
        u32 bucket_index = (u32) (((r64) cur_random_number - (r64) min_r32) / ((r64) max_r32 - (r64) min_r32) * (r64) bucket_size);
        ++buckets[bucket_index];
    }

    r64 chi_square_test_statistic = 0.0;
    for (u32 bucket_index = 0; bucket_index < bucket_size; ++bucket_index) {
        r64 expected_frequency = (r64) max_random_number_counter / (r64) bucket_size;
        r64 observed_frequency = (r64) buckets[bucket_index];
        r64 current_dispersion = pow((observed_frequency - expected_frequency), 2.0) / expected_frequency;
        chi_square_test_statistic += current_dispersion;
    }

    console__log(console, "chi_square_test_statistic for r32: %lf\n", chi_square_test_statistic);
    console__log(console, "total_cys_to_generate_random_numbers for %d r32 numbers: %u\n", max_random_number_counter, total_cys_to_generate_random_numbers);
    console__log(console, "cy for generating 1 r32 number: %lf\n", (r64) total_cys_to_generate_random_numbers / (r64) max_random_number_counter);
}

int WinMain(HINSTANCE app_handle, HINSTANCE prev_instance, LPSTR cmd_line, int show_cmd) {
    struct console console;

    if (!console__init_module(&console, 512)) {
        ExitProcess(EXIT_FAILURE);
    }

    struct random random_generator;
    random__init(&random_generator, 5489ULL);
    
    u32 buckets[NUMBER_OF_BUCKETS] = { 0 };

    console__log(&console, "\n");
    
    u64_tester(&console, &random_generator, buckets, NUMBER_OF_BUCKETS);
    u32_tester(&console, &random_generator, buckets, NUMBER_OF_BUCKETS);
    u16_tester(&console, &random_generator, buckets, NUMBER_OF_BUCKETS);
    i64_tester(&console, &random_generator, buckets, NUMBER_OF_BUCKETS);
    i32_tester(&console, &random_generator, buckets, NUMBER_OF_BUCKETS);
    i16_tester(&console, &random_generator, buckets, NUMBER_OF_BUCKETS);
    r64_tester(&console, &random_generator, buckets, NUMBER_OF_BUCKETS);
    r32_tester(&console, &random_generator, buckets, NUMBER_OF_BUCKETS);

    console__deinit_module(&console);
}
