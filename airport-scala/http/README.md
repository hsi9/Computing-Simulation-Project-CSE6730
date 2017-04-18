# Trace-Viewer Visualizer

Note: We only provide an explanation for the functionality in Trace-Viewer necessary to visualize the output from `airport-scala`. A fuller set of documentation is available in the [Catapult repository](https://github.com/catapult-project/catapult) under [tracing/docs](https://github.com/catapult-project/catapult/tree/master/tracing/docs).

Trace-Viewer is a visualizer which enables us to view the events occuring at each airport as a timeline. The horizontal axis stands for time and the vertical axis stands for airports. Each event is a colored vertical line representing the arrival, departure, landing, or take-off of a single airplane.

## Starting the Visualizer

The Trace-Viewer visualizer is a webpage which reads in a JSON trace file and formats it appropriately for viewing in a browser. Any local HTTP server should suffice; start one of your choice (anything from [this gist](https://gist.github.com/willurd/5720255) should work, but we have explicitly tried the Python3 module successfully) that can serve the `http` directory of the project. We have confirmed that the visualizer loads on the latest release of Google Chrome by simply navigating to `localhost` on the appropriate port, but are aware that Safari encounters an error and cannot load Trace-Viewer. Other browsers have not been tested.

## Generating Output for the Visualizer

The visualizer loads its data from `airport_trace.json` in the same directory. (This can be changed to a different file in `index.html`.) The simulator produces and overwrites this file every time it is run with the `logTraceViewer` option set to `true` in [`src/main/resources/SimulatorConfiguration.yaml`](../src/main/resources/SimulatorConfiguration.yaml). An option to print to standard output instead of `airport_trace.json` is available through the `stdoutTrace` option.

## Navigating the Visualizer Window

The four tool icons in the floating toolbar are, from top to bottom:

- Brush over an area to select all events in the bounding box
- Drag to scroll (tends to work better than scrollbars or scrollwheels)
- Drag vertically to zoom in and out on the horizontal scale
- Drag horizontally to find the timespan between two events. Snaps to events.

