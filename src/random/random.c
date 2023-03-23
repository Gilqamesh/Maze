#include "random.h"

void random__init(struct random* self, u64 seed) {
    self->state_vector[0] = seed;
    for (self->state_vector_index = 1; self->state_vector_index < RANDOM__NN; ++self->state_vector_index) {
        self->state_vector[self->state_vector_index] = (6364136223846793005ULL * (self->state_vector[self->state_vector_index - 1] ^ (self->state_vector[self->state_vector_index - 1] >> 62)) + self->state_vector_index);
    }
}

u64 random__u64(struct random* self) {
    i32 i;
    u64 x;
    static const u64 mag01[2] = {
        0ULL,
        RANDOM__MATRIX_A
    };

    if (self->state_vector_index >= RANDOM__NN) { /* generate RANDOM__NN words at one time */

        for (i = 0; i < RANDOM__NN - RANDOM__MM; ++i) {
            x = (self->state_vector[i] & RANDOM__UM) | (self->state_vector[i + 1] & RANDOM__LM);
            self->state_vector[i] = self->state_vector[i + RANDOM__MM] ^ (x >> 1) ^ mag01[(i32)(x & 1ULL)];
        }

        for (; i < RANDOM__NN - 1; ++i) {
            x = (self->state_vector[i] & RANDOM__UM) | (self->state_vector[i + 1] & RANDOM__LM);
            self->state_vector[i] = self->state_vector[i + (RANDOM__MM - RANDOM__NN)] ^ (x >> 1) ^ mag01[(i32)(x & 1ULL)];
        }

        x = (self->state_vector[RANDOM__NN - 1] & RANDOM__UM) | (self->state_vector[0] & RANDOM__LM);
        self->state_vector[RANDOM__NN - 1] = self->state_vector[RANDOM__MM - 1] ^ (x >> 1) ^ mag01[(i32)(x & 1ULL)];

        self->state_vector_index = 0;
    }

    x = self->state_vector[self->state_vector_index++];

    x ^= (x >> 29) & 0x5555555555555555ULL;
    x ^= (x << 17) & 0x71D67FFFEDA60000ULL;
    x ^= (x << 37) & 0xFFF7EEE000000000ULL;
    x ^= (x >> 43);

    return x;
}

u32 random__u32(struct random* self) {
    return (u32) random__u64(self);
}

u16 random__u16(struct random* self) {
    return (u16) random__u64(self);
}

i64 random__i64(struct random* self) {
    return (i64) random__u64(self);
}

i32 random__i32(struct random* self) {
    return (i32) random__u64(self);
}

i16 random__i16(struct random* self) {
    return (i16) random__u64(self);
}

r64 random__r64(struct random* self) {
    return (r64) ((random__u64(self) >> 11) * (1.0/9007199254740991.0));
}

i64 random__i64_closed(struct random* self, i64 left, i64 right) {
    r64 p = random__r64(self);

    return (i64) ((r64)left + p * ((r64)right - (r64)left));
}

i32 random__i32_closed(struct random* self, i32 left, i32 right) {
    return (i32) random__i64_closed(self, (i64)left, (i64)right);
}

i16 random__i16_closed(struct random* self, i16 left, i16 right) {
    return (i16) random__i64_closed(self, (i64)left, (i64)right);
}

u64 random__u64_closed(struct random* self, u64 left, u64 right) {
    r64 p = random__r64(self);

    return (u64) ((r64)left + p * ((r64)right - (r64)left));
}

u32 random__u32_closed(struct random* self, u32 left, u32 right) {
    return (u32) random__u64_closed(self, (u64)left, (u64)right);
}

u16 random__u16_closed(struct random* self, u16 left, u16 right) {
    return (u16) random__u64_closed(self, (u64)left, (u64)right);
}

r64 random__r64_closed(struct random* self, r64 left, r64 right) {
    r64 p = random__r64(self);

    return left + p * (right - left);
}

r32 random__r32_closed(struct random* self, r32 left, r32 right) {
    return (r32) random__r64_closed(self, (r64)left, (r64)right);
}

r64 random__r64_left_closed(struct random* self, r64 left, r64 right) {
    r64 p = (r64) ((random__u64(self) >> 11) * (1.0/9007199254740992.0));

    return left + p * (right - left);
}

r32 random__r32_left_closed(struct random* self, r32 left, r32 right) {
    return (r32) random__r64_left_closed(self, (r64)left, (r64)right);
}

r64 random__r64_open(struct random* self, r64 left, r64 right) {
    r64 p = (r64) (((r64)(random__u64(self) >> 12) + 0.5) * (1.0/4503599627370496.0));

    return left + p * (right - left);
}

r32 random__r32_open(struct random* self, r32 left, r32 right) {
    return (r32) random__r64_open(self, (r64)left, (r64)right);
}
