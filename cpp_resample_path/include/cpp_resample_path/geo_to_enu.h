#pragma once

namespace cpp_resample_path {

    template<class F = double>
    void geo_to_enu(
        F &ENU_x_meter, F &ENU_y_meter,
        F geo_lon, F geo_lat,
        F geo_lon_home, F geo_lat_home
    )
    {
        const F r_earth = (F)6378137.0;
        const F pi = (F)3.1415926535897932384626433832795;
        const F d2r = (pi / 180.0);
        double d_lon = geo_lon - geo_lon_home;
        double d_lat = geo_lat - geo_lat_home;
        ENU_y_meter = d_lat * d2r * r_earth;
        ENU_x_meter = d_lon * d2r * r_earth * cos(gps_t_lat * d2r);
    };

} // namespace cpp_resample_path
