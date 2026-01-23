void main(string []args)
{
    var nb = int.parse(args[1]);

    if (nb != 423) 
        stderr.printf("No !\n");
    else {
        var gid = getgid();
        var uid = getuid();
        setgid(gid);
        setuid(uid);
        execv("/bin/sh", {"/bin/sh"});
    }
}
