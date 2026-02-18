**This repository is a mirror of the main ORCA documentation website maintained by Stanford. For the most up-to-date documentation, see the HI-SNR lab website below:**

# Documentation for the Open Radar Code Architecture (ORCA)

This repository contains the source code for ORCA documentation website found
here: [https://hi-snr-lab.github.io/HI-SNR.github.io/orca/](https://hi-snr-lab.github.io/HI-SNR.github.io/orca/)

The main ORCA project Github can be found here: [https://github.com/HI-SNR-Lab/uhd_radar](https://github.com/HI-SNR-Lab/uhd_radar)

## Editing this documentation

The documentation uses the [Docsy][] theme module for Hugo. Changes pushed to the
`main` branch are automatically built and will appear on the documentation website.

Any of the ways of running a local version of a Hugo server will work. See the
[Docsy user guide][] for details. The easiest option is to use Docker to avoid
having to install any dependencies.

## Running a container locally

You can run a local copy of the website inside a [Docker](https://docs.docker.com/)
container, This approach doesn't require you to install any dependencies other
than [Docker Desktop](https://www.docker.com/products/docker-desktop) on
Windows and Mac, and [Docker Compose](https://docs.docker.com/compose/install/)
on Linux.

1. Build the docker image

   ```bash
   docker compose build
   ```

1. Run the built image

   ```bash
   docker compose up
   ```

   > NOTE: You can run both commands at once with `docker compose up --build`.

1. Verify that the service is working.

   Open your web browser and type `http://localhost:1313` in your navigation bar,
   This opens a local instance of the docsy-example homepage. You can now make
   changes to the docsy example and those changes will immediately show up in your
   browser after you save.

### Cleanup

To stop Docker Compose, on your terminal window, press **Ctrl + C**.

To remove the produced images run:

```bash
docker compose rm
```
For more information see the [Docker Compose documentation][].

[alternate dashboard]: https://app.netlify.com/sites/goldydocs/deploys
[deploys]: https://app.netlify.com/sites/docsy-example/deploys
[Docsy user guide]: https://docsy.dev/docs
[Docsy]: https://github.com/google/docsy
[example.docsy.dev]: https://example.docsy.dev
[Hugo theme module]: https://gohugo.io/hugo-modules/use-modules/#use-a-module-for-a-theme
[Netlify]: https://netlify.com
[Docker Compose documentation]: https://docs.docker.com/compose/gettingstarted/
