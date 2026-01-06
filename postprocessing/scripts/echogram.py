# /// script
# requires-python = ">=3.12"
# dependencies = [
#     "dask==2025.12.0",
#     "distributed==2025.12.0",
#     "holoviews==1.22.1",
#     "hvplot==0.12.2",
#     "marimo",
#     "matplotlib==3.10.8",
#     "numpy==2.4.0",
#     "pandas==2.3.3",
#     "ruamel-yaml==0.19.0",
#     "scipy==1.16.3",
#     "simple-parsing==0.1.7",
#     "xarray==2025.12.0",
#     "zarr==3.1.5",
# ]
# ///

import marimo

__generated_with = "0.18.4"
app = marimo.App(width="medium")


@app.cell
def _():
    import marimo as mo
    return (mo,)


@app.cell
def _():
    from dataclasses import dataclass
    from simple_parsing import ArgumentParser
    return ArgumentParser, dataclass


@app.cell
def _():
    from dask.distributed import Client, LocalCluster

    client = Client() # Note that `memory_limit` is the limit **per worker**.
    # n_workers=4,
    #                 threads_per_worker=1,
    #                 memory_limit='3GB'
    client # If you click the dashboard link in the output, you can monitor real-time progress and get other cool visualizations.
    return


@app.cell
def _():
    import copy
    import sys
    import os.path
    import zarr
    import xarray as xr
    import numpy as np
    import dask.array as da

    import matplotlib.pyplot as plt
    import hvplot.xarray
    import hvplot.pandas
    import holoviews as hv
    from holoviews import opts
    import scipy.constants

    # for parsing GPS and stdout log files
    import re
    import pandas as pd
    return copy, np, os, plt, re, scipy, sys, xr


@app.cell
def _(sys):
    sys.path.append("..")
    sys.path.append("../../preprocessing/")
    sys.path.append("../notebooks/")

    import processing_dask as pr
    import gps_processing as gps
    import Geostacking as geostack
    import plot_dask
    import processing as old_processing

    from generate_chirp import generate_chirp
    return generate_chirp, geostack, gps, pr


@app.cell(hide_code=True)
def _(mo):
    mo.md(r"""
    ## Time Parsing Functions
    """)
    return


@app.cell
def _(re):
    def parse_stdout_times(line):
        # Extract the timestamp from lines that contain specific phrases
        if '[START] Beginning main loop' in line:
            pattern = r'(\d+\.\d+)'
            match = re.search(pattern, line)
            if match:
                return float(match.group(1))

        elif '[TX] Closing file' in line:
            pattern = r'(\d+\.\d+)'
            match = re.search(pattern, line)
            if match:
                return float(match.group(1))

        return None
    return (parse_stdout_times,)


@app.cell
def _(parse_stdout_times):
    def start_and_stop_from_log(logstring):
        lines = logstring.split("\n")
        results = []
        for line in lines:
            parsed = parse_stdout_times(line.strip())
            if parsed:
                results.append(parsed)

        # handle the case where we didn't get the stop time
        if len(results)==1:
            results.append(float("NaN"))
    
        return {"start": results[0], "stop": results[1]}
    return (start_and_stop_from_log,)


@app.cell
def _(np):
    def get_distance_along_track(df):
        displacements = np.linalg.norm(df.iloc[1:,:].loc[:, ["ecefx", "ecefy", "ecefz"]].to_numpy() - \
        df.iloc[:-1,:].loc[:, ["ecefx", "ecefy", "ecefz"]].to_numpy(), axis=1)
        return np.cumsum(displacements)
    return (get_distance_along_track,)


@app.cell(hide_code=True)
def _(mo):
    mo.md(r"""
    ## Data Selection
    """)
    return


@app.cell(hide_code=True)
def _(mo):
    mo.md(r"""
    #### Argument Parsing

    Logic based on https://docs.marimo.io/guides/scripts/#simple-parsing
    """)
    return


@app.cell
def _(ArgumentParser, dataclass):
    parser = ArgumentParser()

    @dataclass
    class InputLocation:
        """Input location arguments.
        - path: directory containing test files
        - prefix: prefix of files for the desired radargram
        """

        path: str  # Path to folder contain test files
        prefix: str  # Test name prefix for desired stest

    parser.add_arguments(InputLocation, dest="input")
    parser.add_argument("--imagepath", type=str, default="./", help="directory in which to save radargram images")
    parser.add_argument("--zarrpath", type=str, default="./", help="directory in which to save ZARR files")
    return (parser,)


@app.cell
def _(mo, parser):
    def _():
        zarr_base_location="/Users/thatch/Projects/SORA/before-and-after-test/test_tmp_zarr_cache/"

        path = "/Volumes/CI1/"
        test = "20251209_200750" # leg 3 - down at 350, 58 db

        def parse_args():
            if mo.running_in_notebook():
                # set default values for the command-line arguments when running as a notebook
                # in this case, just pass along the values defined above as the defaults.
                # notebook users can edit those values when running in notebook mode.
                imagepath = "."
                return path, test, imagepath, zarr_base_location
            else:
                args = parser.parse_args()
                return args.input.path, args.input.prefix, args.imagepath, args.zarrpath

        return parse_args

    parse_args = _()
    return (parse_args,)


@app.cell
def _(parse_args):
    path, test, imagepath, zarr_base_location = parse_args()
    return imagepath, path, test, zarr_base_location


@app.cell
def _(path, test):
    prefix = path + test
    return (prefix,)


@app.cell
def _(prefix, zarr_base_location):
    print(f"Processing {prefix}* files")
    print(f"ZARR cache location: {zarr_base_location}")
    return


@app.cell
def _(pr, prefix, xr, zarr_base_location):
    # resave data as zarr for dask processing
    #zarr_path = pr.save_radar_data_to_zarr(prefix, zarr_base_location=zarr_base_location)
    zarr_path = pr.save_radar_data_to_zarr(prefix, zarr_base_location=zarr_base_location,expected_base_name_regex=False)

    # open zarr file, adjust chunk size to be 10 MB - 1 GB based on sample rate/bit depth
    raw = xr.open_zarr(zarr_path, chunks={"pulse_idx": 1000})
    return (raw,)


@app.cell(hide_code=True)
def _(mo):
    mo.md(r"""
    ## GPS Processing
    """)
    return


@app.cell
def _(geostack, get_distance_along_track, gps, start_and_stop_from_log):
    def georeference_raw_data(raw):
        gpsdf = gps.extract_locations_from_gpspipe( raw.gpspipe_log )
        res = start_and_stop_from_log(raw.stdout_log)
        duration = res["stop"] - res["start"]
        print(f"Duration: {duration:.1f} s")

        gpsdf["slow_time"] = gpsdf["unix_time"] - res["start"]

        georaw = geostack.add_distance_to_xarray(raw, 
                                             raw.slow_time.compute(), 
                                             gpsdf["slow_time"].iloc[1:],
                                             get_distance_along_track(gpsdf))

        return georaw
    return (georeference_raw_data,)


@app.cell
def _(georeference_raw_data, raw):
    georaw = georeference_raw_data(raw)
    return (georaw,)


@app.cell(hide_code=True)
def _(mo):
    mo.md(r"""
    ## Processing Parameters
    """)
    return


@app.cell
def _(np, scipy):
    #zero_sample_idx = 36 # X310, fs = 20 MHz
    #zero_sample_idx = 63 # X310, fs = 50 MHz
    zero_sample_idx = 159 # B205mini, fs = 56 MHz for the SORA used by GLASS
    #zero_sample_idx = 166 # B205mini, fs = 20 MHz

    nstack = 1000 # number of pulses to stack
    lenstack = 5 # number of meters to stack over

    modify_rx_window = False # set to true if you want to window the reference chirp only on receive, false uses ref chirp as transmitted in config file
    rx_window = "blackman" # what you want to change the rx window to if modify_rx_window is true

    dielectric_constant = 3.17# ice (air = 1, 66% velocity coax = 2.2957)
    #dielectric_constant = 2.2957 # COAX (air = 1, 66% velocity coax = 2.2957)
    #dielectric_constant = 1.0
    sig_speed = scipy.constants.c / np.sqrt(dielectric_constant)
    return (
        dielectric_constant,
        lenstack,
        modify_rx_window,
        nstack,
        rx_window,
        sig_speed,
        zero_sample_idx,
    )


@app.cell(hide_code=True)
def _(mo):
    mo.md(r"""
    ### Generate Reference Chirp
    """)
    return


@app.cell
def _(copy, generate_chirp, modify_rx_window, raw, rx_window):
    if modify_rx_window:
        config = copy.deepcopy(raw.config)
        config['GENERATE']['window'] = rx_window
    else:
        config = raw.config

    chirp_ts, ref_chirp = generate_chirp(config)
    return (ref_chirp,)


@app.cell(hide_code=True)
def _(mo):
    mo.md(r"""
    ### View raw pulse in time domain to check for clipping
    """)
    return


@app.cell
def _(np, raw, test):
    single_pulse_raw = raw.radar_data[{'pulse_idx': 100}].compute()
    plot1 = np.real(single_pulse_raw).hvplot.line(x='fast_time', color='red') * np.imag(single_pulse_raw).hvplot.line(x='fast_time')

    plot1 = plot1.opts(xlabel='Fast Time (s)', ylabel='Raw Amplitude', title=f"Pulse 100 from {test}")
    plot1
    return


@app.cell(hide_code=True)
def _(mo):
    mo.md(r"""
    ## Clean and stack data
    """)
    return


@app.cell
def _(georaw, geostack, lenstack, pr):
    geostacked = pr.fill_errors(georaw, error_fill_value=0.0) # fill receiver errors with 0s
    geostacked = geostack.stack(geostacked, lenstack)
    return (geostacked,)


@app.cell
def _(nstack, pr, raw):
    stacked = pr.fill_errors(raw, error_fill_value=0.0) # fill receiver errors with 0s
    stacked = pr.stack(stacked, nstack) # stack 
    return (stacked,)


@app.cell(hide_code=True)
def _(mo):
    mo.md(r"""
    ## Pulse Compress Data
    """)
    return


@app.cell
def _(geostacked, np, pr, ref_chirp, sig_speed, stacked, xr, zero_sample_idx):
    compressed = pr.pulse_compress(stacked, ref_chirp,
                                   fs=stacked.config['GENERATE']['sample_rate'],
                                   zero_sample_idx=zero_sample_idx,
                                   signal_speed=sig_speed)

    geocompressed = pr.pulse_compress(geostacked, ref_chirp,
                                   fs=stacked.config['GENERATE']['sample_rate'],
                                   zero_sample_idx=zero_sample_idx,
                                   signal_speed=sig_speed)

    compressed_power = xr.apply_ufunc(
        lambda x: 20*np.log10(np.abs(x)),
        compressed,
        dask="parallelized"
    )

    geocompressed_power = xr.apply_ufunc(
        lambda x: 20*np.log10(np.abs(x)),
        geocompressed,
        dask="parallelized"
    )
    return compressed_power, geocompressed_power


@app.cell(hide_code=True)
def _(mo):
    mo.md(r"""
    ## View 1D pulse compressed data
    """)
    return


@app.cell
def _(compressed_power, dielectric_constant, np, test):
    plot1D = np.mean(compressed_power.radar_data, axis=0).hvplot.line(label="Mean of All Pulses")
    plot1D = plot1D * compressed_power.radar_data[0,:].hvplot.line(label="First Pulse")
    plot1D = plot1D * compressed_power.radar_data[-1,:].hvplot.line(label="Last Pulse")

    # relevant options: xlim(-80,1000)

    plot1D = plot1D.opts(xlabel='Reflection Distance (m)', ylabel='Return Power (dB)')
    plot1D = plot1D.opts(title=test+" (ϵ="+str(dielectric_constant)+")")
    plot1D.opts(xlim=(-50,2300), ylim=(-120, -25), show_grid=True)
    return


@app.cell(hide_code=True)
def _(mo):
    mo.md(r"""
    ## View 2D pulse compressed data (radargram)
    """)
    return


@app.cell
def _(compressed_power):
    # USING HOLOVIEWS (sometimes breaks)
    plot2D = compressed_power.swap_dims({'pulse_idx': 'slow_time', 'travel_time': 'reflection_distance'}).hvplot.quadmesh(x='slow_time', cmap='inferno', flip_yaxis=True)
    # relevant options: ylim=(100,-50), clim=(-90,-40)

    plot2D.opts(xlabel='Slow Time (s)', ylabel='Depth (m)', clabel='Return Power (dB)')
    plot2D.opts(ylim=(-10, 2000), clim=(-115, -70))
    return


@app.cell
def _(geocompressed_power):
    # USING HOLOVIEWS (sometimes breaks)
    geoplot2D = geocompressed_power.swap_dims({'travel_time': 'reflection_distance'}).hvplot\
        .quadmesh(x='distance_bin', cmap='inferno', flip_yaxis=True, width=1000)
    # relevant options: ylim=(100,-50), clim=(-90,-40)

    geoplot2D.opts(xlabel='Distance along track (m)', ylabel='Depth (m)', clabel='Return Power (dB)')
    geoplot2D.opts(ylim=(-10, 2000), clim=(-100, -65))
    return


@app.cell(hide_code=True)
def _(mo):
    mo.md(r"""
    ### Matplotlib Plots
    """)
    return


@app.cell
def _(compressed_power, imagepath, mo, os, plt, test):
    def _():
        # USING MATPLOTLIB (sometimes takes a while)
        fig, ax = plt.subplots(1,1, figsize=(14,6), facecolor='white')

        p = ax.pcolormesh(compressed_power.slow_time, \
                          compressed_power.reflection_distance, \
                          compressed_power.radar_data.transpose(), \
                          shading='auto', cmap='inferno',\
                         # vmin = -140, vmax=-60
                         )
        ax.invert_yaxis()
        clb = fig.colorbar(p, ax=ax)
        clb.set_label('Return Power (dB)')
        ax.set_xlabel('Slow Time (s)')
        ax.set_ylabel('Distance to Reflector (m)')
        # relevant options: ax.set_ylim=(100,-50), ax.set_xlim=(0, 1), vmin=-90, vmax=40
        ax.set_ylim(2000, -50)
        plt.title(f"{test} Radargram")

        if mo.running_in_notebook():
            return ax
        else:
            imagefile = os.path.join(imagepath, f"{test}-radargram.png")
            plt.savefig(imagefile, bbox_inches='tight')
            print(f"Saved {imagefile}")

    _()
    return


@app.cell
def _(geocompressed_power, imagepath, mo, os, plt, test):
    def _():
        # USING MATPLOTLIB (sometimes takes a while)
        fig, ax = plt.subplots(1,1, figsize=(14,6), facecolor='white')

        p = ax.pcolormesh(geocompressed_power.distance_bin, \
                          geocompressed_power.reflection_distance, \
                          geocompressed_power.radar_data.transpose(), \
                          shading='auto', cmap='inferno',\
                          vmin = -100, vmax=-20
                         )
        ax.invert_yaxis()
        clb = fig.colorbar(p, ax=ax)
        clb.set_label('Return Power (dB)')
        ax.set_xlabel('Distance along track (m)')
        ax.set_ylabel('Distance to Reflector (m)')
        # relevant options: ax.set_ylim=(100,-50), ax.set_xlim=(0, 1), vmin=-90, vmax=40
        ax.set_ylim(2000, -50)
        plt.title(f"{test} Radargram - Georeferenced")

        if mo.running_in_notebook():
            return ax
        else:
            imagefile = os.path.join(imagepath, f"{test}-radargram-georeferenced.png")
            plt.savefig(imagefile, bbox_inches='tight')
            print(f"Saved {imagefile}")

    _()
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
