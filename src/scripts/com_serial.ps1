param(
    [string]$port = "COM5",
    [UInt16]$action = 9,
    [uint16]$sdata = 0
)

$sport = New-Object System.IO.Ports.SerialPort
$sport.PortName = $port
$sport.BaudRate = 9600
$sport.Parity = "None"
$sport.DataBits = 8
$sport.StopBits = 1

$action = $action -band 0x0F
$sdata = $sdata -shl 4
$sdata = $sdata -bor $action

[byte[]] $buffer = ($sdata -band 0xFF), ($sdata -shr 8)

$sport.Open()
$sport.Write($buffer, 0, 2)

if ($action -eq 9) {
    for ($i = 0; $i -lt 7; $i++) {
        Write-Host (($sport.ReadByte() -bor ($sport.ReadByte() -shl 8)) -shr 4)
    }
}

$sport.Close()
