basic.forever(function () {
    basic.showNumber(9)
    basic.pause(1000)
    basic.showString("A")
    basic.pause(1000)
    basic.showIcon(IconNames.Heart)
    basic.pause(1000)
    basic.showIcon(IconNames.Happy)
    basic.pause(1000)
    basic.showLeds(`
        # # # # #
        # . . . #
        # . . . #
        # . . . #
        # # # # #
        `)
    basic.pause(1000)
    basic.clearScreen()
    led.plotBarGraph(
    10,
    100
    )
    basic.pause(500)
    led.plotBarGraph(
    20,
    100
    )
    basic.pause(500)
    led.plotBarGraph(
    30,
    100
    )
    basic.pause(500)
    led.plotBarGraph(
    40,
    100
    )
    basic.pause(500)
    led.plotBarGraph(
    50,
    100
    )
    basic.pause(500)
    led.plotBarGraph(
    60,
    100
    )
    basic.pause(500)
    led.plotBarGraph(
    70,
    100
    )
    basic.pause(500)
    led.plotBarGraph(
    80,
    100
    )
    basic.pause(500)
    led.plotBarGraph(
    90,
    100
    )
    basic.pause(500)
    led.plotBarGraph(
    100,
    100
    )
    basic.pause(3000)
    basic.clearScreen()
})
