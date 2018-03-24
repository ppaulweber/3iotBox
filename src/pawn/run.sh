
for i in {0..59}; do
    echo "    <field>"
    echo "        name       = field$(expr $i + 2)"
    echo "        alias      = data$i"
    echo "        title      = Receipt (Beleg) Data $i"
    echo "        type       = string"
    echo "        byteofs    = $(expr $i \* 60 + 64)"
    echo "        max        = 60"
    echo "        default    = "
    echo "        edit       = 5"
    echo "    </field>"
done
