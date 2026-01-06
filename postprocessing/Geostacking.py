import xarray as xr
import numpy as np
from scipy.interpolate import interp1d


def add_distance_to_xarray(data_array, slow_time_values, gps_times, gps_distances):
    """
    Add distance-along-track coordinate to xarray by interpolating GPS data.
    
    Parameters
    ----------
    data_array : xr.DataArray
        2D xarray with dimensions (sample_idx, ...) and slow_time coordinate
    slow_time_values : array-like
        The slow_time values corresponding to each sample_idx
    gps_times : array-like
        Time values from GPS data
    gps_distances : array-like
        Distance along track values from GPS data
        
    Returns
    -------
    xr.DataArray
        Data array with added 'distance_along_track' coordinate
    """
    # Create interpolation function for distance along track
    # Use bounds_error=False to extrapolate for times outside GPS range
    distance_interp = interp1d(
        gps_times, 
        gps_distances, 
        kind='linear',
        bounds_error=False,
        fill_value='extrapolate'
    )
    
    # Interpolate distances for each slow_time
    distances = distance_interp(slow_time_values)
    
    # Add as a coordinate to the data array
    data_with_distance = data_array.assign_coords(
        distance_along_track=('pulse_idx', distances)
    )
    
    return data_with_distance


def group_and_average_by_distance(data_array, bin_size=5.0, distance_coord='distance_along_track'):
    """
    Group xarray columns by distance along track and compute mean.
    
    Parameters
    ----------
    data_array : xr.DataArray
        Data array with distance_along_track coordinate
    bin_size : float
        Size of distance bins in meters (default: 5.0)
    distance_coord : str
        Name of the distance coordinate (default: 'distance_along_track')
        
    Returns
    -------
    xr.DataArray
        Averaged data grouped by distance bins
    """
    # Get the distance values
    distances = data_array[distance_coord].values
    
    # Create bins
    min_dist = np.floor(distances.min())
    max_dist = np.ceil(distances.max())
    bins = np.arange(min_dist, max_dist + bin_size, bin_size)
    
    # Assign each sample to a bin (using bin centers as labels)
    bin_indices = np.digitize(distances, bins) - 1
    bin_centers = bins[:-1] + bin_size / 2
    
    # Clip to valid range
    bin_indices = np.clip(bin_indices, 0, len(bin_centers) - 1)
    
    # Add bin assignment as a coordinate
    data_with_bins = data_array.assign_coords(
        distance_bin=('pulse_idx', bin_centers[bin_indices])
    )
    
    # Group by distance bin and compute mean
    grouped = data_with_bins.groupby('distance_bin').mean(dim='pulse_idx')
    
    return grouped

def stack(data: xr.Dataset, dist_stack: float):
    #"""
    #Stack (average) data along the slow time axis in chunks of n_stack chirps
    #All relevant coordinates (i.e. slow_time, pulse_idx) are taken as their
    #minimum value in the stack.
    #"""
    #return data.coarsen({'pulse_idx': n_stack},
    #             boundary='trim',
    #             coord_func='min').mean()
    return group_and_average_by_distance(data, 
                                         bin_size=dist_stack, 
                                         distance_coord='distance_along_track')


def process_data(data_array, slow_time_values, gps_times, gps_distances, bin_size=5.0):
    """
    Complete pipeline: add distance coordinate and group by distance bins.
    
    Parameters
    ----------
    data_array : xr.DataArray
        2D xarray with dimensions (sample_idx, ...) and slow_time coordinate
    slow_time_values : array-like
        The slow_time values corresponding to each sample_idx
    gps_times : array-like
        Time values from GPS data
    gps_distances : array-like
        Distance along track values from GPS data
    bin_size : float
        Size of distance bins in meters (default: 5.0)
        
    Returns
    -------
    xr.DataArray
        Averaged data grouped by distance bins
    """
    # Step 1: Add distance coordinate
    data_with_distance = add_distance_to_xarray(
        data_array, slow_time_values, gps_times, gps_distances
    )
    
    # Step 2: Group and average by distance
    result = group_and_average_by_distance(data_with_distance, bin_size=bin_size)
    
    return result