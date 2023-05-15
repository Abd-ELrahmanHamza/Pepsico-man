Imports IrrKlang

Module Module1

    Sub Main()

        ' start up the engine
        Dim engine As New ISoundEngine

        ' To play a sound, we only to call play2D(). The second parameter
        ' tells the engine to play it looped.

        engine.Play2D("../../media/getout.ogg", True)

        Console.Out.WriteLine("")
        Console.Out.WriteLine("Hello World")

        Do
            Console.Out.WriteLine("Press any key to play some sound, press 'q' to quit.")

            ' play a single sound
            engine.Play2D("../../media/bell.wav")

        Loop While _getch() <> 113 ' until 'the key "q" is pressed

    End Sub

    ' some simple function for reading keys from the console
    <System.Runtime.InteropServices.DllImport("msvcrt")> _
    Public Function _getch() As Integer
    End Function

End Module
