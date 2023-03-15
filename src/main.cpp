#include <mbed.h>

#include <array>
#include <chrono>

using namespace std;
using namespace std::chrono_literals;
using namespace std::chrono;

namespace {

constexpr int frequency = 60;
constexpr int can_id = 144;

constexpr microseconds min_pulse_width = 500us;
constexpr microseconds max_pulse_width = 2500us;
constexpr microseconds default_pulse_width = (max_pulse_width + min_pulse_width) / 2;

}  // namespace

microseconds byte_to_pulse_width(uint8_t b) {
  constexpr int span = max_pulse_width.count() - min_pulse_width.count();
  return microseconds((int32_t)b * span / 255) + min_pulse_width;
}

int main() {
  array<PwmOut, 8> pwms{{{PA_1}, {PA_3}, {PA_4}, {PA_6}, {PA_7}, {PA_8}, {PA_9}, {PA_10}}};
  CAN can{PA_11, PA_12, 1000000};

  for (auto&& pwm : pwms) {
    pwm.period_us(1'000'000 / frequency);
  }

  for (auto&& pwm : pwms) {
    pwm.pulsewidth_us(default_pulse_width.count());
  }

  while (1) {
    CANMessage msg;
    if (can.read(msg) && msg.format == CANFormat::CANStandard && msg.id == can_id && msg.len == pwms.size()) {
      printf("yay message\n");
      for (size_t i = 0; i < pwms.size(); i++) {
        int pulse_width_us = byte_to_pulse_width(msg.data[i]).count();
        pwms[i].pulsewidth_us(pulse_width_us);
      }
    }
  }
}