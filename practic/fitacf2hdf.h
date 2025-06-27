#include "defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hdf5.h"
#include "rtypes.h"
#include "dmap.h"
#include "rprm.h"
#include "fitdata.h"
#include "fitread.h"

typedef struct {
    hid_t rec_id;

    hid_t radar_id;
    hid_t revision_id;    
    hid_t origin_id;      
    hid_t time_prm_id;    
    hid_t stat_id;        
    hid_t noise_prm_id;   
    hid_t pulse_id;
    hid_t lag_id;

    hid_t fit_id;
    hid_t revision_fit_id;
    hid_t noise_fit_id;
    hid_t rng_id;
    hid_t xrng_id;
    hid_t elv_id;
} HDF5GroupHandles;

HDF5GroupHandles create_record_groups(hid_t records_id, int record_index, int mode_count);
void close_record_groups(HDF5GroupHandles handles);

static void write_scalar_dataset(hid_t group, const char *name, hid_t type, const void *value);
static void write_string_dataset(hid_t group, const char *name, const char *str);
void write_radarparm(const HDF5GroupHandles *handles, const struct RadarParm *prm);

hid_t create_fitrange_type();
void write_fitdata(HDF5GroupHandles *handles, const struct FitData *fit, int nrang);
void write_fitrange_group(hid_t rng_group, const struct FitRange *data, int count);
void write_fitelv_group(hid_t elv_group, const struct FitElv *data, int count);
