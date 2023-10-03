import tkinter as tk
import plotly.graph_objects as go
import numpy as np
import random

def simulate_TDoA_with_surrounding():
    x_transmitter, y_transmitter, z_transmitter = random.uniform(0.4, 0.6), random.uniform(0.4, 0.6), random.uniform(0.4, 0.6)

    x_receiver = [x_transmitter + random.uniform(-0.2, 0.2) for _ in range(4)]
    y_receiver = [y_transmitter + random.uniform(-0.2, 0.2) for _ in range(4)]
    z_receiver = [z_transmitter + random.uniform(-0.2, 0.2) for _ in range(4)]


    scatter = go.Scatter3d(
        x=x_receiver + [x_transmitter],
        y=y_receiver + [y_transmitter],
        z=z_receiver + [z_transmitter],
        mode='markers',
        marker=dict(size=[12, 12, 12, 12, 20], color=['blue', 'blue', 'blue', 'blue', 'red'])
    )

    mesh = go.Mesh3d(
        x=[],
        y=[],
        z=[],
        opacity=0.5,
        colorbar_title='z'
    )

    layout = go.Layout(
        updatemenus=[{
            'buttons': [
                {'args': [None, {'frame': {'duration': 0, 'redraw': True}, 'fromcurrent': True}], 'label': 'Play', 'method': 'animate'}
            ],
            'direction': 'left',
            'pad': {'r': 10, 't': 87},
            'showactive': False,
            'type': 'buttons',
            'x': 0.1,
            'xanchor': 'right',
            'y': 0,
            'yanchor': 'top'
        }]
    )

    fig = go.Figure(data=[scatter, mesh], layout=layout)

    frames = []
    arrival_times = [None, None, None, None]

    frame_count = 2000

    # voodoo magic for mesh rendering -- see https://plotly.com/python/3d-charts/
    for frame_num, r in enumerate(np.linspace(0.01, 0.5, frame_count)):  # radius from 0.01 to 0.5, enumerated by frame count
        # Create a sphere by specifying its x, y, and z coordinates
        phi = np.linspace(0, 2 * np.pi, 20)
        theta = np.linspace(0, np.pi, 20)
        phi, theta = np.meshgrid(phi, theta)

        x = x_transmitter + r * np.sin(theta) * np.cos(phi)
        y = y_transmitter + r * np.sin(theta) * np.sin(phi)
        z = z_transmitter + r * np.cos(theta)

        x = x.flatten()
        y = y.flatten()
        z = z.flatten()

        i = []
        j = []
        k = []

        for t in range(len(theta) - 1):
            for p in range(len(phi[0]) - 1):
                i.append(t * len(phi[0]) + p)
                j.append((t + 1) * len(phi[0]) + p)
                k.append(t * len(phi[0]) + p + 1)

        # Check if the expanding sphere has reached any receiver nodes
        for idx in range(4):
            distance_to_receiver = np.sqrt((x_receiver[idx] - x_transmitter)**2 + (y_receiver[idx] - y_transmitter)**2 + (z_receiver[idx] - z_transmitter)**2)
            if distance_to_receiver <= r and arrival_times[idx] is None:
                arrival_times[idx] = frame_num

        frames.append(go.Frame(data=[scatter, go.Mesh3d(x=x, y=y, z=z, i=i, j=j, k=k, opacity=0.5)], name=f"Frame {frame_num}"))

    fig.frames = frames

    # Show the plot
    fig.show()
    print("Receiver Arrival Times (in frames):", arrival_times)

def draw_circle(canvas, x, y, r, color="blue", tag=""):
    return canvas.create_oval(x - r, y - r, x + r, y + r, fill=color, tags=tag)
