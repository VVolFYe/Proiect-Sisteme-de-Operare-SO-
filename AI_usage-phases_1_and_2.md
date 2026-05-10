# AI Usage (First phase)

I used ChatGPT only for the two helper functions required in the project specification:

- parse_condition(const char *input, char *field, char *op, char *value)
- match_condition(Report *r, const char *field, const char *op, const char *value)

For parse_condition() I asked for a function that splits a condition written as field:operator:value into its three parts.

For match_condition() I described my Report structure and the field types and asked for a function that checks if a report satisfies a condition. (I attached the code from report.h since that only contains the struct I used for this project.)

I reviewed both generated functions line by line, adapted them to my own code and field names, and tested them with my filter command. It did not work first try but with 2 more prompts I solved the error and then used those functions in the final filter function. (the functions are in commands.h as well as aux for filter)

# AI Usage (Second phase)

For the second phase I tried not to use AI that much and instead used Stack Overflow.
I took the structure from Stack Overflow for some functions. (mainly for handlers i guess)

Then I there were some functions that I used that didn't make sense to me and made AI explain them and realized that some of them were not needed. (for example calling fflush at some point)

Regarding the remove_district function I did not use AI at all because I made it entirely in the lab so I hope it should be okay.

