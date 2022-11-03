BOOT_TOOLS:mx8-nxp-bsp = "imx-boot-tools"

# From u-boot-imx/meta-freescale
do_deploy:append:mx8-nxp-bsp() {
    # Deploy u-boot-nodtb.bin and XX.dtb for mkimage to generate boot binary
    if [ -n "${UBOOT_CONFIG}" ]; then
        for config in ${UBOOT_MACHINE}; do
            machine_idx=$(expr $machine_idx + 1);
            for type in ${UBOOT_CONFIG}; do
                type_idx=$(expr $type_idx + 1);
                if [ $type_idx -eq $machine_idx ]; then
                    install -d ${DEPLOYDIR}/${BOOT_TOOLS}
                    # When sign is enabled the final DTB should be copied from deploy dir
                    if [ "${UBOOT_SIGN_ENABLE}" = "1" ]; then
                        install -m 0644 ${DEPLOY_DIR_IMAGE}/${UBOOT_DTB_IMAGE} ${DEPLOYDIR}/${BOOT_TOOLS}/${UBOOT_DTB_NAME}
                    else
                        install -m 0644 ${B}/${config}/arch/arm/dts/${UBOOT_DTB_NAME} ${DEPLOYDIR}/${BOOT_TOOLS}
                    fi
                    install -m 0644 ${B}/${config}/spl/u-boot-spl-nodtb.bin ${DEPLOYDIR}/${BOOT_TOOLS}/u-boot-spl-nodtb.bin-${MACHINE}-${type}
                    install -m 0644 ${B}/${config}/u-boot-nodtb.bin ${DEPLOYDIR}/${BOOT_TOOLS}/u-boot-nodtb.bin-${MACHINE}-${type}
                fi
            done
            unset type_idx
        done
        unset machine_idx
    else
        install -d ${DEPLOYDIR}/${BOOT_TOOLS}
        # When sign is enabled the final DTB should be copied from deploy dir
        if [ "${UBOOT_SIGN_ENABLE}" = "1" ]; then
            install -m 0644 ${DEPLOY_DIR_IMAGE}/${UBOOT_DTB_IMAGE} ${DEPLOYDIR}/${BOOT_TOOLS}/${UBOOT_DTB_NAME}
        else
            install -m 0644 ${B}/arch/arm/dts/${UBOOT_DTB_NAME} ${DEPLOYDIR}/${BOOT_TOOLS}
        fi
        install -m 0644 ${B}/spl/u-boot-spl-nodtb.bin ${DEPLOYDIR}/${BOOT_TOOLS}/u-boot-spl-nodtb.bin-${MACHINE}-
        install -m 0644 ${B}/u-boot-nodtb.bin ${DEPLOYDIR}/${BOOT_TOOLS}/u-boot-nodtb.bin-${MACHINE}-
        cd ${DEPLOYDIR}
        ln -sf u-boot-${MACHINE}.bin u-boot-${MACHINE}.bin-
        ln -sf u-boot-spl.bin-${MACHINE} u-boot-spl.bin-${MACHINE}-
    fi
}

do_deploy:append:stm32mp1common() {
    if [ "${UBOOT_SIGN_ENABLE}" != "1" -a -n "${UBOOT_CONFIG}" ]; then
        for config in ${UBOOT_MACHINE}; do
            machine_idx=$(expr $machine_idx + 1);
            for type in ${UBOOT_CONFIG}; do
                type_idx=$(expr $type_idx + 1);
                if [ $type_idx -eq $machine_idx ]; then
                    for devicetree in ${UBOOT_DEVICETREE}; do
                        install -m 0644 ${B}/${config}/u-boot-nodtb.bin ${DEPLOYDIR}
                        install -m 0644 ${B}/${config}/arch/arm/dts/${devicetree}.dtb ${DEPLOYDIR}/u-boot-${devicetree}.dtb
                        # Latest takes preference as we don't support multiple dtbs/fips
                        ln -sf u-boot-${devicetree}.dtb ${DEPLOYDIR}/${UBOOT_DTB_BINARY}
                    done
                fi
            done
            unset type_idx
        done
        unset machine_idx
    fi
}