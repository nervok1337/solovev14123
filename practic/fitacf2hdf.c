#include "fitacf2hdf.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s fitacf_file\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    hid_t file_id = H5Fcreate("out.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    hid_t records_id = H5Gcreate2(file_id, "/records", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    struct RadarParm *prm = RadarParmMake();
    struct FitData *fit = FitMake();

    if (!prm || !fit) {
        fprintf(stderr, "Memory allocation failed.\n");
        fclose(fp);
        return 1;
    }

    int record_index = 0;
    const int MODE_COUNT = 5;

    while (FitFread(fp, prm, fit) != -1) {
        int nrange = prm->nrang;

        HDF5GroupHandles handles = create_record_groups(records_id, record_index, MODE_COUNT);

        write_radarparm(&handles, prm);
        write_fitdata(&handles, fit, nrange);

        close_record_groups(handles);
        record_index++;
    }

    H5Gclose(records_id);
    H5Fclose(file_id);
    fclose(fp);

    RadarParmFree(prm);
    FitFree(fit);

    printf("Создано записей: %d\n", record_index);
    return 0;
}


HDF5GroupHandles create_record_groups(hid_t records_id, int record_index, int mode_count) {
    HDF5GroupHandles handles;
    char record_name[64];
    char mode_name[16];

    snprintf(record_name, sizeof(record_name), "record_%d", record_index);
    handles.rec_id = H5Gcreate2(records_id, record_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // Основная группа radar_parameters
    handles.radar_id = H5Gcreate2(handles.rec_id, "radar_parameters", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // Вложенные группы radar_parameters
    handles.revision_id  = H5Gcreate2(handles.radar_id, "revision", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    handles.origin_id    = H5Gcreate2(handles.radar_id, "origin", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    handles.time_prm_id  = H5Gcreate2(handles.radar_id, "time", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    handles.stat_id      = H5Gcreate2(handles.radar_id, "stat", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    handles.noise_prm_id = H5Gcreate2(handles.radar_id, "noise", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // pulse и lag
    handles.pulse_id = H5Gcreate2(handles.radar_id, "pulse", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    handles.lag_id   = H5Gcreate2(handles.radar_id, "lag", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // fit группа и вложенные
    handles.fit_id      = H5Gcreate2(handles.rec_id, "fit", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    handles.revision_fit_id = H5Gcreate2(handles.fit_id, "revision", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    handles.noise_fit_id    = H5Gcreate2(handles.fit_id, "noise", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    handles.rng_id      = H5Gcreate2(handles.fit_id, "rng", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    handles.xrng_id     = H5Gcreate2(handles.fit_id, "xrng", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    handles.elv_id      = H5Gcreate2(handles.fit_id, "elv", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // Создание подгрупп для modes в rng, xrng, elv
    for (int mode = 0; mode < mode_count; mode++) {
        snprintf(mode_name, sizeof(mode_name), "mode_%d", mode);
        hid_t rng_mode  = H5Gcreate2(handles.rng_id,  mode_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        hid_t xrng_mode = H5Gcreate2(handles.xrng_id, mode_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        hid_t elv_mode  = H5Gcreate2(handles.elv_id,  mode_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

        H5Gclose(rng_mode);
        H5Gclose(xrng_mode);
        H5Gclose(elv_mode);
    }

    return handles;
}
void close_record_groups(HDF5GroupHandles handles) {
    // Вложенные radar_parameters
    H5Gclose(handles.revision_id);
    H5Gclose(handles.origin_id);
    H5Gclose(handles.time_prm_id);
    H5Gclose(handles.stat_id);
    H5Gclose(handles.noise_prm_id);

    // Основные radar_parameters
    H5Gclose(handles.pulse_id);
    H5Gclose(handles.lag_id);
    H5Gclose(handles.radar_id);

    // fit вложенные
    H5Gclose(handles.revision_fit_id);
    H5Gclose(handles.noise_fit_id);
    H5Gclose(handles.rng_id);
    H5Gclose(handles.xrng_id);
    H5Gclose(handles.elv_id);
    H5Gclose(handles.fit_id);

    // Главная группа записи
    H5Gclose(handles.rec_id);
}

static void write_scalar_dataset(hid_t group, const char *name, hid_t type, const void *value) {
    hid_t space_id = H5Screate(H5S_SCALAR);
    hid_t dset_id = H5Dcreate2(group, name, type, space_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    H5Dwrite(dset_id, type, H5S_ALL, H5S_ALL, H5P_DEFAULT, value);
    H5Dclose(dset_id);
    H5Sclose(space_id);
}
static void write_string_dataset(hid_t group, const char *name, const char *str) {
    if (!str) str = "";
    hid_t type = H5Tcopy(H5T_C_S1);
    H5Tset_size(type, strlen(str));
    hid_t space = H5Screate(H5S_SCALAR);
    hid_t dset = H5Dcreate2(group, name, type, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    H5Dwrite(dset, type, H5S_ALL, H5S_ALL, H5P_DEFAULT, str);
    H5Dclose(dset);
    H5Sclose(space);
    H5Tclose(type);
}
void write_radarparm(const HDF5GroupHandles *handles, const struct RadarParm *prm) {
    // revision (char major, minor)
    write_scalar_dataset(handles->revision_id, "major", H5T_NATIVE_CHAR, &prm->revision.major);
    write_scalar_dataset(handles->revision_id, "minor", H5T_NATIVE_CHAR, &prm->revision.minor);

    // origin (char code, char* time, char* command)
    write_scalar_dataset(handles->origin_id, "code", H5T_NATIVE_CHAR, &prm->origin.code);
    write_string_dataset(handles->origin_id, "time", prm->origin.time);
    write_string_dataset(handles->origin_id, "command", prm->origin.command);

    // cp, stid (в radar_id, т.к. у тебя их нет в отдельных группах)
    write_scalar_dataset(handles->radar_id, "cp", H5T_NATIVE_SHORT, &prm->cp);
    write_scalar_dataset(handles->radar_id, "stid", H5T_NATIVE_SHORT, &prm->stid);

    // time (int16 yr, mo, dy, hr, mt, sc; int32 us)
    write_scalar_dataset(handles->time_prm_id, "year", H5T_NATIVE_SHORT, &prm->time.yr);
    write_scalar_dataset(handles->time_prm_id, "month", H5T_NATIVE_SHORT, &prm->time.mo);
    write_scalar_dataset(handles->time_prm_id, "day", H5T_NATIVE_SHORT, &prm->time.dy);
    write_scalar_dataset(handles->time_prm_id, "hour", H5T_NATIVE_SHORT, &prm->time.hr);
    write_scalar_dataset(handles->time_prm_id, "minute", H5T_NATIVE_SHORT, &prm->time.mt);
    write_scalar_dataset(handles->time_prm_id, "second", H5T_NATIVE_SHORT, &prm->time.sc);
    write_scalar_dataset(handles->time_prm_id, "usec", H5T_NATIVE_INT, &prm->time.us);

    // txpow, nave, atten, lagfr, smsep, ercod (в radar_id)
    write_scalar_dataset(handles->radar_id, "txpow", H5T_NATIVE_SHORT, &prm->txpow);
    write_scalar_dataset(handles->radar_id, "nave", H5T_NATIVE_SHORT, &prm->nave);
    write_scalar_dataset(handles->radar_id, "atten", H5T_NATIVE_SHORT, &prm->atten);
    write_scalar_dataset(handles->radar_id, "lagfr", H5T_NATIVE_SHORT, &prm->lagfr);
    write_scalar_dataset(handles->radar_id, "smsep", H5T_NATIVE_SHORT, &prm->smsep);
    write_scalar_dataset(handles->radar_id, "ercod", H5T_NATIVE_SHORT, &prm->ercod);

    // stat (int16 agc, lopwr)
    write_scalar_dataset(handles->stat_id, "agc", H5T_NATIVE_SHORT, &prm->stat.agc);
    write_scalar_dataset(handles->stat_id, "lopwr", H5T_NATIVE_SHORT, &prm->stat.lopwr);

    // noise (float search, mean)
    write_scalar_dataset(handles->noise_prm_id, "search", H5T_NATIVE_FLOAT, &prm->noise.search);
    write_scalar_dataset(handles->noise_prm_id, "mean", H5T_NATIVE_FLOAT, &prm->noise.mean);

    // channel, bmnum, bmazm, scan, rxrise (radar_id)
    write_scalar_dataset(handles->radar_id, "channel", H5T_NATIVE_SHORT, &prm->channel);
    write_scalar_dataset(handles->radar_id, "bmnum", H5T_NATIVE_SHORT, &prm->bmnum);
    write_scalar_dataset(handles->radar_id, "bmazm", H5T_NATIVE_FLOAT, &prm->bmazm);
    write_scalar_dataset(handles->radar_id, "scan", H5T_NATIVE_SHORT, &prm->scan);
    write_scalar_dataset(handles->radar_id, "rxrise", H5T_NATIVE_SHORT, &prm->rxrise);

    // intt (int16 sc, int32 us) — создаем и закрываем группу intt прямо здесь
    hid_t intt_grp = H5Gcreate2(handles->radar_id, "intt", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    write_scalar_dataset(intt_grp, "sc", H5T_NATIVE_SHORT, &prm->intt.sc);
    write_scalar_dataset(intt_grp, "us", H5T_NATIVE_INT, &prm->intt.us);
    H5Gclose(intt_grp);

    // txpl, mpinc, mppul, mplgs, mplgexs, nrang, frang, rsep, xcf, tfreq, offset, ifmode (radar_id)
    write_scalar_dataset(handles->radar_id, "txpl", H5T_NATIVE_SHORT, &prm->txpl);
    write_scalar_dataset(handles->radar_id, "mpinc", H5T_NATIVE_SHORT, &prm->mpinc);
    write_scalar_dataset(handles->radar_id, "mppul", H5T_NATIVE_SHORT, &prm->mppul);
    write_scalar_dataset(handles->radar_id, "mplgs", H5T_NATIVE_SHORT, &prm->mplgs);
    write_scalar_dataset(handles->radar_id, "mplgexs", H5T_NATIVE_SHORT, &prm->mplgexs);
    write_scalar_dataset(handles->radar_id, "nrang", H5T_NATIVE_SHORT, &prm->nrang);
    write_scalar_dataset(handles->radar_id, "frang", H5T_NATIVE_SHORT, &prm->frang);
    write_scalar_dataset(handles->radar_id, "rsep", H5T_NATIVE_SHORT, &prm->rsep);
    write_scalar_dataset(handles->radar_id, "xcf", H5T_NATIVE_SHORT, &prm->xcf);
    write_scalar_dataset(handles->radar_id, "tfreq", H5T_NATIVE_SHORT, &prm->tfreq);
    write_scalar_dataset(handles->radar_id, "offset", H5T_NATIVE_SHORT, &prm->offset);
    write_scalar_dataset(handles->radar_id, "ifmode", H5T_NATIVE_SHORT, &prm->ifmode);

    // mxpwr, lvmax (int32)
    write_scalar_dataset(handles->radar_id, "mxpwr", H5T_NATIVE_INT, &prm->mxpwr);
    write_scalar_dataset(handles->radar_id, "lvmax", H5T_NATIVE_INT, &prm->lvmax);

    // combf (char*), строка
    write_string_dataset(handles->radar_id, "combf", prm->combf);

    // Запись массива pulse
    if (prm->pulse && prm->mppul > 0) {
        hsize_t pulse_dims[1] = { (hsize_t)prm->mppul };
        hid_t pulse_space = H5Screate_simple(1, pulse_dims, NULL);
        hid_t pulse_dset = H5Dcreate2(handles->pulse_id, "values", H5T_NATIVE_SHORT, pulse_space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        H5Dwrite(pulse_dset, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, prm->pulse);
        H5Dclose(pulse_dset);
        H5Sclose(pulse_space);
    }
    // Запись массива lag[2] 
    if (prm->lag[0] && prm->lag[1] && prm->mplgs > 0) {
        hsize_t lag_dims[1] = { (hsize_t)prm->mplgs };
        hid_t lag_space = H5Screate_simple(1, lag_dims, NULL);

        hid_t lag0_dset = H5Dcreate2(handles->lag_id, "first", H5T_NATIVE_SHORT, lag_space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        H5Dwrite(lag0_dset, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, prm->lag[0]);
        H5Dclose(lag0_dset);

        hid_t lag1_dset = H5Dcreate2(handles->lag_id, "second", H5T_NATIVE_SHORT, lag_space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        H5Dwrite(lag1_dset, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT, prm->lag[1]);
        H5Dclose(lag1_dset);

        H5Sclose(lag_space);
    }
}

hid_t create_fitrange_type() {
    hid_t type = H5Tcreate(H5T_COMPOUND, sizeof(struct FitRange));

    H5Tinsert(type, "v",         HOFFSET(struct FitRange, v),         H5T_NATIVE_DOUBLE);
    H5Tinsert(type, "v_err",     HOFFSET(struct FitRange, v_err),     H5T_NATIVE_DOUBLE);
    H5Tinsert(type, "p_0",       HOFFSET(struct FitRange, p_0),       H5T_NATIVE_DOUBLE);
    H5Tinsert(type, "p_l",       HOFFSET(struct FitRange, p_l),       H5T_NATIVE_DOUBLE);
    H5Tinsert(type, "p_l_err",   HOFFSET(struct FitRange, p_l_err),   H5T_NATIVE_DOUBLE);
    H5Tinsert(type, "p_s",       HOFFSET(struct FitRange, p_s),       H5T_NATIVE_DOUBLE);
    H5Tinsert(type, "p_s_err",   HOFFSET(struct FitRange, p_s_err),   H5T_NATIVE_DOUBLE);
    H5Tinsert(type, "w_l",       HOFFSET(struct FitRange, w_l),       H5T_NATIVE_DOUBLE);
    H5Tinsert(type, "w_l_err",   HOFFSET(struct FitRange, w_l_err),   H5T_NATIVE_DOUBLE);
    H5Tinsert(type, "w_s",       HOFFSET(struct FitRange, w_s),       H5T_NATIVE_DOUBLE);
    H5Tinsert(type, "w_s_err",   HOFFSET(struct FitRange, w_s_err),   H5T_NATIVE_DOUBLE);
    H5Tinsert(type, "phi0",      HOFFSET(struct FitRange, phi0),      H5T_NATIVE_DOUBLE);
    H5Tinsert(type, "phi0_err",  HOFFSET(struct FitRange, phi0_err),  H5T_NATIVE_DOUBLE);
    H5Tinsert(type, "sdev_l",    HOFFSET(struct FitRange, sdev_l),    H5T_NATIVE_DOUBLE);
    H5Tinsert(type, "sdev_s",    HOFFSET(struct FitRange, sdev_s),    H5T_NATIVE_DOUBLE);
    H5Tinsert(type, "sdev_phi",  HOFFSET(struct FitRange, sdev_phi),  H5T_NATIVE_DOUBLE);
    H5Tinsert(type, "qflg",      HOFFSET(struct FitRange, qflg),      H5T_NATIVE_INT);
    H5Tinsert(type, "gsct",      HOFFSET(struct FitRange, gsct),      H5T_NATIVE_INT);
    H5Tinsert(type, "nump",      HOFFSET(struct FitRange, nump),      H5T_NATIVE_CHAR);

    return type;
}
void write_fitdata(HDF5GroupHandles *handles, const struct FitData *fit, int nrang) {
    // Записать версию ревизии
    write_scalar_dataset(handles->revision_fit_id, "major", H5T_NATIVE_INT, &fit->revision.major);
    write_scalar_dataset(handles->revision_fit_id, "minor", H5T_NATIVE_INT, &fit->revision.minor);

    // Записать шумовые параметры
    write_scalar_dataset(handles->noise_fit_id, "vel", H5T_NATIVE_DOUBLE, &fit->noise.vel);
    write_scalar_dataset(handles->noise_fit_id, "skynoise", H5T_NATIVE_DOUBLE, &fit->noise.skynoise);
    write_scalar_dataset(handles->noise_fit_id, "lag0", H5T_NATIVE_DOUBLE, &fit->noise.lag0);

    // Записать массив данных по диапазонам rng
    write_fitrange_group(handles->rng_id, fit->rng, nrang);

    // Записать массив данных по расширенным диапазонам xrng
    write_fitrange_group(handles->xrng_id, fit->xrng, nrang);

    // Записать данные углов elevation
    write_fitelv_group(handles->elv_id, fit->elv, nrang);
}
void write_fitrange_group(hid_t rng_group, const struct FitRange *data, int count) {
    if (!data || count <= 0) return;

    // Создать тип данных для FitRange
    hid_t type = create_fitrange_type();

    // Размер массива для записи
    hsize_t dims[1] = { (hsize_t)count };
    hid_t space = H5Screate_simple(1, dims, NULL);

    // Создать 5 групп-модификаций и записать данные в каждую
    for (int i = 0; i < 5; ++i) {
        char mod_group_name[64];
        snprintf(mod_group_name, sizeof(mod_group_name), "mod_%d", i);

        hid_t mod_group = H5Gcreate2(rng_group, mod_group_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

        // Записать датасет с данными FitRange
        hid_t dset = H5Dcreate2(mod_group, "data", type, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        H5Dwrite(dset, type, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);

        H5Dclose(dset);
        H5Gclose(mod_group);
    }

    H5Sclose(space);
    H5Tclose(type);
}
void write_fitelv_group(hid_t elv_group, const struct FitElv *data, int count) {
    if (count <= 0) return;

    // Создать составной тип для FitElv
    hid_t type = H5Tcreate(H5T_COMPOUND, sizeof(struct FitElv));
    H5Tinsert(type, "normal", HOFFSET(struct FitElv, normal), H5T_NATIVE_DOUBLE);
    H5Tinsert(type, "low",    HOFFSET(struct FitElv, low),    H5T_NATIVE_DOUBLE);
    H5Tinsert(type, "high",   HOFFSET(struct FitElv, high),   H5T_NATIVE_DOUBLE);

    // Размер массива для записи
    hsize_t dims[1] = { (hsize_t)count };
    hid_t space = H5Screate_simple(1, dims, NULL);

    struct FitElv *empty_data = NULL;
    if (!data) {
        empty_data = (struct FitElv *)calloc(count, sizeof(struct FitElv));
        data = empty_data;
    }

    // Создать 5 групп-модификаций и записать данные в каждую
    for (int i = 0; i < 5; ++i) {
        char mod_group_name[64];
        snprintf(mod_group_name, sizeof(mod_group_name), "mod_%d", i);

        hid_t mod_group = H5Gcreate2(elv_group, mod_group_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

        // Записать датасет с данными FitElv
        hid_t dset = H5Dcreate2(mod_group, "data", type, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        H5Dwrite(dset, type, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);

        H5Dclose(dset);
        H5Gclose(mod_group);
    }

    H5Sclose(space);
    H5Tclose(type);
}
