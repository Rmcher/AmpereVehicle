# AmpereVehicle

Ampere Vehicle/TDPS 2022-2023 (Group 2) - Team-26

The **Ampere Vehicle** project is a team project mainly for a rover completed by a group of students who enjoy working with electronic equipment in the **Team Design and Project Skills (TDPS) UESTC3010** at the UESTC, Glasgow College. The rover can navigate bumpy cobblestones, recognize arrows, track lines, drop a ping pong ball in a trash can, and communicate with a computer. The repository mainly contains the following contents: two main control programs of the rover based on the STM32G474RE main control board (for Patio 1 and Patio 2), image recognition code, and line inspection code based on OpenMV. Because of the particularity of our deployment, the codes of devices including ultrasound and gyroscopes are encapsulated in the main control program.

## Prerequisites

You should have a relevant background in **embedded development**. In terms of software capabilities, you need to have a basic understanding of **C language** and **python**. In terms of hardware, due to the limitations of the repository, you need to have the ability to build hardware from scratch according to the instruction file.

You need to use the [Keil Studio Cloud](https://studio.keil.arm.com/auth/login/) online compiler as the interpreter of the main control program.
And You also need download the official [IDE of OpenMV](https://openmv.io/pages/download) as the compiler for the python file of the vision module.

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone whose code was used
* Inspiration
* etc
