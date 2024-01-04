
#include <stdio.h>
#include <inttypes.h>

#include <ctl.h>

static const char* json = "{ \"dir\": \"a_path\", \"bitMask\": 4, \"useInternal\": true }";

int main(int argc, char** argv) {
    int             ret;
    struct ctl_cfg  cfg;
    const char*     file;

    if (argc > 1) {
        file = argv[1];
        fprintf(stdout, "Reading JSON from file: %s\n", file);
    }

    ret = ctl_parse_config(json, &cfg);
    if (ret < 0) {
        fprintf(stderr, "Error: Unable to parse JSON into config\n");
        return 1;
    }

    fprintf(stdout, "JSON successfully parsed into config:\n");
    fprintf(stdout, "{\n");
    fprintf(stdout, "    \"dir\": %s,\n", cfg.dir);
    fprintf(stdout, "    \"bit_mask\": %"PRIu32"\n", cfg.bit_mask);
    fprintf(stdout, "    \"use_internal\": %d\n", cfg.use_internal);
    fprintf(stdout, "}\n");

    return 0;
}
