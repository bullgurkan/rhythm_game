#include "KeyBind.hpp"

KeyBind::KeyBind(std::vector<KeyAction> actions, std::unordered_map<int, int>* keyAmountToColorId) : actions{ actions }, keyAmountToColorId{ keyAmountToColorId }{}

