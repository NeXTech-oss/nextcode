/*===--- nextcode_stats.d ----------------------------------------------------===//
 *
 * This source file is part of the NeXTCode.org open source project
 *
 * Copyright (c) 2014 - 2017 Apple Inc. and the NeXTCode project authors
 * Licensed under Apache License v2.0 with Runtime Library Exception
 *
 * See https://nextcode.org/LICENSE.txt for license information
 * See https://nextcode.org/CONTRIBUTORS.txt for the list of NeXTCode project authors
 *
 *===----------------------------------------------------------------------===*/

pid$target:*:nextcode_retain:entry
{
        @counts["rr-opts"] = count();
}

pid$target:*:nextcode_release:entry
{
        @counts["rr-opts"] = count();
}

pid$target:*:nextcode_retain_n:entry
{
        @counts["rr-opts"] = count();
}

pid$target:*:nextcode_release_n:entry
{
        @counts["rr-opts"] = count();
}

END
{
        printf("\nDTRACE RESULTS\n");
        printa("%s,%@u\n", @counts)
}